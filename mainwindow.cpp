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

  button = menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(1); // to SQLite
  connect(button, SIGNAL(triggered(bool)), this, SLOT(convertToSql()));

  // connect для convert to CSV!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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
    bool kk = query.exec("SELECT * FROM " + name + ";");

    model.setQuery(query);

    statusBar()->showMessage(QString("%1, %2, %3").arg(query.isActive()).arg(query.isSelect()).arg(kk));
  }
}

void MainWindow::slotOpen()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "SQLite files(*.sqlite *.db);;CSV files(*.csv)");

  currentFile = QFileInfo(name);

  if (name != "")         // если реально выбрали файл, а не нажали Cancel
  {
    if (isOpen)           // если нажали Open 2 раза подряд
    {
      makeDisabled();     // приводим видимость виджетов в исходное "пустое" состояние
      model.clear();      // чистим модель и, следовательно, представление
      csvModel.clear(); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

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

  QStringList lstheaders;
  processRecord(file, lstheaders);

  csvModel.insertColumns(0, lstheaders.size()); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  for (int j = 0; j < lstheaders.size(); ++j)
    csvModel.setHeaderData(j, Qt::Horizontal, lstheaders.at(j)); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  qDebug() << lstheaders.size() << csvModel.rowCount() << csvModel.columnCount(); // !!!!!!!!!!!!!!!!!!!!!!!!!!

  int rows = 0;
  while (!file.atEnd())
  {
    processRecord(file, lstheaders);

    csvModel.insertRows(rows, 1); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for (int j = 0; j < lstheaders.size(); ++j)
    {
      bool kk = csvModel.setData(csvModel.index(rows, j), lstheaders.at(j)); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      qDebug() << lstheaders << kk << rows; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    ++rows;
  }

  file.close();
}

void MainWindow::processRecord(QFile& file, QStringList& lstheaders)
{
  int quoteCount = 1;                // количество " в строке
  QString strheaders;                // первая строка csv

  while (quoteCount % 2 != 0)        // кавычек нечетное кол-во (случай: xxx,"x \n x",xxx)
  {
    QString temp(file.readLine());   // считывает до первого \n
    temp.remove("\r\n");
    quoteCount = temp.count('\"');   // сколько раз встретилась "

    if (strheaders.isEmpty())
      strheaders = temp;
    else    // strheaders += temp;
     strheaders += '\n' + temp;     // не уверен, что нужно \n !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  //  qDebug() << strheaders; //________________________________________________________________________________
  }

  lstheaders = strheaders.split(',');  // разбили строку по , на поля
  int i = 0;

 // qDebug() << lstheaders; //____________________________________________________________________________________

  while (i != lstheaders.size())          // пока не пройдем весь список
  {
    QString temp = lstheaders.at(i);      // получили текущую строку
    quoteCount = temp.count('\"');        // сколько раз встретилась "

    if (quoteCount %2 != 0)
    {
      lstheaders.replace(i+1, temp+','+lstheaders.at(i+1));  // сливаю 2 строки. ВОЗМОЖНА ОШИБКА i+1=size!!!!!!!!!!!!
      lstheaders.removeAt(i);                                // надеюсь сработает
    }
    else
      ++i;
  }

  QString temp = lstheaders.at(0);               // смотрим самую первую строку (поле записи)
  if (temp.at(0) == '\"')                        // смотрим его первый символ
    temp.remove(0, 1);                           // удаляем, если это "

  temp = lstheaders.at(lstheaders.size() - 1);   // смотрим самую последнюю строку (поле записи)
  if (temp.at(temp.size() - 1) == '\"')          // смотрим ее последний символ
    temp.chop(1);                                // удаляем, если это "

  for (int i = 0; i < lstheaders.size(); ++i)    // идем по всем строкам
  {
    temp = lstheaders.at(i);
    int quotes = temp.indexOf("\"\"");;          // есть ли в строке ""?

    while (quotes != -1)                         // в строке есть ""
    {
      temp.remove(quotes, 1);                    // заменяем на ", если нашли
      quotes = temp.indexOf("\"\"");
    }
  }
}

void MainWindow::convertToSql()
{
  dialog = new Dialog(this);
  dialog->exec();
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


