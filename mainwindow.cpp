#include "mainwindow.h"
#include <QFileDialog>
#include "dialog.h"

void MainWindow::createMenuBar(MainWindow* parent)
{
  menuBar = new QMenuBar(parent);
  QMenu* menuFile = new QMenu("File", menuBar);
  QMenu* menuConvert = new QMenu("Convert to...", menuFile);

  menuBar->addAction(menuFile->menuAction());

  menuFile->addAction("Open");                      // 0
  menuFile->addAction(menuConvert->menuAction());   // 1
  menuFile->addSeparator();                         // 2
  menuFile->addAction("Exit");                      // 3

  menuConvert->addAction("CSV");                    // 1.0
  menuConvert->addAction("SQLite");                 // 1.1

  menuBar->actions().at(0)->menu()->actions().at(1)->setEnabled(false);
  menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(0)->setEnabled(false);
  menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(1)->setEnabled(false);
}

void MainWindow::createToolBar(MainWindow* parent)
{
  toolBar = new QToolBar(parent);
  tablesBox = new QComboBox(toolBar);

  toolBar->setMovable(false);
  toolBar->addWidget(tablesBox);
  tablesBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  tablesBox->setEnabled(false);
}

void MainWindow::createConnections()
{
  QAction* button = menuBar->actions().at(0)->menu()->actions().at(0);   // open
  connect(button, SIGNAL(triggered(bool)), this, SLOT(slotOpen()));

  button = menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(0); // to CSV
  connect(button, SIGNAL(triggered(bool)), this, SLOT(convertToCsv()));

  button = menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(1); // to SQLite
  connect(button, SIGNAL(triggered(bool)), this, SLOT(convertToSql()));

  button = menuBar->actions().at(0)->menu()->actions().at(3);           // exit application
  connect(button, SIGNAL(triggered(bool)), qApp, SLOT(quit()));

  // выбор показа таблицы
  connect(tablesBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(setModelForTable(const QString&)));
}

void MainWindow::makeEnabled(const QString& str)
{
  menuBar->actions().at(0)->menu()->actions().at(1)->setEnabled(true);  // convert to...

  if (str == "sql")
  {
    menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(0)->setEnabled(true); // to csv
    tablesBox->setEnabled(true);
  }
  else
    menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(1)->setEnabled(true); // to sql
}

void MainWindow::makeDisabled()
{
  menuBar->actions().at(0)->menu()->actions().at(1)->setEnabled(false);  // convert to...
  menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(0)->setEnabled(false); // to csv
  menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(1)->setEnabled(false); // to sql
  tablesBox->clear();
  tablesBox->setEnabled(false);
}

void MainWindow::setModelForTable(const QString & name)
{
  if (!(name == "")) // чтобы не было реакции при очистке через makeDisabled()
  {
    QSqlQuery query(db);
    query.exec("SELECT * FROM " + name + ";");

    model.setQuery(query);
  }
}

void MainWindow::slotOpen()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "SQLite files(*.sqlite *.db);;CSV files(*.csv)");

  if (name != "")         // если реально выбрали файл, а не нажали Cancel
  {
    currentFile = QFileInfo(name);

    if (isOpen)           // если нажали Open 2 раза подряд
    {
      makeDisabled();     // приводим видимость виджетов в исходное "пустое" состояние
      model.clear();      // чистим модель и, следовательно, представление
      csvModel.clear();

      if (db.isOpen())    // если в прошлый Open открывалась база
        db.close();

      isOpen = false;     // сбрасываем состояние
    }

    if (name.endsWith(".sqlite") || name.endsWith(".db"))
    {
      view->setModel(&model);
      isOpen = true;
      makeEnabled("sql");
      openSql(name);
    }
    else
      if (name.endsWith(".csv"))
      {
        view->setModel(&csvModel);
        isOpen = true;
        makeEnabled("csv");
        openCsv(name);
      }
  }
}

void MainWindow::openSql(const QString& name)
{
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(name);
  db.open();

  tablesBox->addItems(db.tables());
}

void MainWindow::openCsv(const QString& name)
{
  QFile file(name);
  file.open(QIODevice::ReadOnly);

  QStringList fields; // в первом случае - заголовки столбцов, во втором - поля записи
  processRecord(file, fields);

  csvModel.insertColumns(0, fields.size());

  for (int j = 0; j < fields.size(); ++j)
    csvModel.setHeaderData(j, Qt::Horizontal, fields.at(j));

// qDebug() << lstheaders.size() << csvModel.rowCount() << csvModel.columnCount(); // !!!!!!!!!!!!!!!!!!!!!!!!!!

  int rows = 0;
  while (!file.atEnd()) // если в файле была только строка шапки, то сюда даже не зайдем
  {
    processRecord(file, fields);
    csvModel.insertRows(rows, 1);

    for (int j = 0; j < fields.size(); ++j)
    {
      csvModel.setData(csvModel.index(rows, j), fields.at(j));

// qDebug() << lstheaders << rows; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    ++rows;
  }

  file.close();
}

QString MainWindow::handleFileString(QFile& file)
{
  int quoteCount = 1;                // количество " в строке. 1 - чтобы зайти в цикл
  QString strRecordLine;             // строка таблицы

  while (quoteCount % 2 != 0)        // кавычек нечетное кол-во (случай: xxx,"x \n x",xxx)
  {
    QString temp(file.readLine());   // считывает до первого \n

qDebug() << '1' << temp; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    temp.remove("\r\n");
    if (temp.at(temp.size()-1) == '\n')
      temp.chop(1);

//qDebug() << '2' << temp; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    quoteCount = temp.count('\"');   // сколько раз встретилась "

    if (strRecordLine.isEmpty())
      strRecordLine = temp;
    else    // strheaders += temp;
      strRecordLine += '\n' + temp;     // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }

qDebug() << '3' << strRecordLine; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  return strRecordLine;
}

void MainWindow::handleString(QStringList& lstRecordLine)
{
  int quoteCount = 0;
  QStringList::iterator cur = lstRecordLine.begin();

  while (cur != lstRecordLine.end())          // пока не пройдем весь список
  {
    QString temp = *cur;                  // получили текущее слово
    quoteCount = temp.count('\"');        // сколько раз встретилась "

    if (quoteCount %2 != 0)
    {
      cur->clear();
      (*cur) = temp + ',' + (*(cur+1)); // сливаю 2 строки.  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     // lstRecordLine.replace(i, temp+','+lstRecordLine.at(i+1));
      lstRecordLine.erase(cur + 1);  // надеюсь сработает   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }
    else
      ++cur;
  }
}

void MainWindow::processRecord(QFile& file, QStringList& lstRecordLine)
{
  lstRecordLine = handleFileString(file).split(',');  // разбили строку по , на поля
  handleString(lstRecordLine);

  QString temp = lstRecordLine.at(0);               // смотрим самое первое слово
  if (temp.at(0) == '\"')                        // смотрим его первый символ
  {
    temp.remove(0, 1);                           // удаляем, если это "
    lstRecordLine.replace(0, temp);
  }

  temp = lstRecordLine.at(lstRecordLine.size()-1);   // смотрим самое последнее слову
  if (temp.at(temp.size() - 1) == '\"')          // смотрим ее последний символ
  {
    temp.chop(1);                                // удаляем, если это "
    lstRecordLine.replace(temp.size()-1, temp);
  }

  for (int i = 0; i < lstRecordLine.size(); ++i)    // идем по всем словам
  {
    temp = lstRecordLine.at(i);
    QString::iterator cur = temp.begin();

    int j = 0;
    while (cur != temp.end())
    {
      if ((*cur == '\"') && (*(cur+1) == '\"')) // в слове может быть только от двух кавычек подряд
        temp.remove(j+1, 1);

      ++cur;
      ++j;
    }

    lstRecordLine.replace(i, temp);
  }
}

void MainWindow::convertToSql()
{
  dialog = new Dialog(this);
  dialog->exec();
}

void MainWindow::convertToCsv()
{
  QString dirFile = currentFile.absolutePath() + QDir::separator() +tablesBox->currentText();
  QString name = QFileDialog::getSaveFileName(this, "Explorer", dirFile, "CSV files(*.csv)");

  if (name != "")
  {
    QFile file(name);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate); // транкейт !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for(int j = 0; j < model.columnCount(); ++j)
    {
      handleWordToCsv(file, model.headerData(j, Qt::Horizontal).toString());

      if (j != model.columnCount() - 1)
        file.putChar(',');
      else
        file.putChar('\n'); // блокнот не может перевести на новую строку !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    for (int i = 0; i < model.rowCount(); ++i)
    {
      for (int j = 0; j < model.columnCount(); ++j)
      {
        handleWordToCsv(file, model.data(model.index(i, j)).toString());

        if (j != model.columnCount() - 1)
          file.putChar(',');
      }

      if (i != model.rowCount() - 1)
        file.putChar('\n');
    }

    file.close();
  }
}

void MainWindow::handleWordToCsv(QFile& file, QString word)
{
  bool flagSymbols = false;

  if (word.contains(QRegExp("[\\\",\\\n]")))   // в строке есть особые символы
  {
    file.putChar('\"');
    flagSymbols = true;
  }

  for(int i = 0; i < word.size(); ++i)
  {
    file.putChar(word.at(i).toLatin1());

    if (word.at(i) == '\"')                               // попали на кавычку
      file.putChar(word.at(i).toLatin1());                // дублируем ее
  }

  if (flagSymbols)
    file.putChar('\"');
}

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
  resize(1024, 768);

  createMenuBar(this);
  setMenuBar(menuBar);

  createToolBar(this);
  addToolBar(Qt::TopToolBarArea, toolBar);

  createConnections();

  view = new QTableView(this);
  setCentralWidget(view);
}


