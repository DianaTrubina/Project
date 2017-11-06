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

  if (name != "")         // если реально выбрали файл, а не нажали Cancel
  {
    if (isOpen)           // если нажали Open 2 раза подряд
    {
      makeDisabled();     // приводим видимость виджетов в исходное "пустое" состояние
      model.clear();      // чистим модель и, следовательно, представление (перенести перед if(isOpen))

      if (db.isOpen())    // если в прошлый Open открывалась база
      {
        db.close();
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // db.remove("defaultConnection") надо добавлять (т.к. остается в списке соединений)?
      }
      else                // в прошлый раз база не открывалась
      {} //действия если был открыт csv (будет пустым)!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

      isOpen = false;     // сбрасываем состояние
    }

    if (name.endsWith(".sqlite") || name.endsWith(".db"))
    {
      isOpen = true;
      makeEnabled("sql");
      openSql(name);
    }
    else
      if (name.endsWith(".csv"))
      {
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
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void MainWindow::convertToSql()
{
  dialog = new Dialog(this);
  dialog->resize(400,400);
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
  view->setModel(&model);
  setCentralWidget(view);
}


