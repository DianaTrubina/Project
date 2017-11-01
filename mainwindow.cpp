#include "mainwindow.h"
#include <QFileDialog>
QMenuBar* MainWindow::createMenuBar(MainWindow* parent)
{
  QMenuBar* bar = new QMenuBar(parent);
  QMenu* menuFile = new QMenu("File", bar);
  QMenu* menuConvert = new QMenu("Convert to...", menuFile);

  bar->addAction(menuFile->menuAction());

  menuFile->addAction("Open");                      // 0
  menuFile->addAction(menuConvert->menuAction());   // 1
  menuFile->addAction("Close");                     // 2
  menuFile->addSeparator();                         // 3
  menuFile->addAction("Exit");                      // 4

  menuConvert->addAction("csv");
  menuConvert->addAction("sqlite");

  return bar;
}

void MainWindow::slotOpen()
{
 // Explorer* win = new Explorer(1, this);

  //connect(win, SIGNAL(openSql(const QString&, const QString&)), this, SLOT(openSql(const QString&, const QString&)));
  //connect(win, SIGNAL(openCsv(const QString&, const QString&)), this, SLOT(openCsv(const QString&, const QString&)));

 // win->show();
    auto name = QFileDialog::getOpenFileName(this, "Open db", "", "Sqlite files(*.sqlite);;CSV file(*.csv)");
    if (name.endsWith(".sqlite")){
        openSql("", name);
    } else{
        openCsv("", name);
    }
}

void MainWindow::openCsv(const QString& path, const QString& name)
{
}

void MainWindow::openSql(const QString& path, const QString& name)
{
  SQLmodel.setTable(name); // просто имя или с путем?
  SQLmodel.select();
}

void MainWindow::slotClose()
{
  // отключение от базы, файла csv, очистка моделей
}

void MainWindow::createConnections(QMenuBar* bar)
{
  QAction* button = bar->actions().at(0)->menu()->actions().at(0);   // open
  connect(button, SIGNAL(triggered(bool)), this, SLOT(slotOpen()));

  bar->actions().at(0)->menu()->actions().at(1)->setEnabled(false);
  bar->actions().at(0)->menu()->actions().at(2)->setEnabled(false);
  button = bar->actions().at(0)->menu()->actions().at(2);            // close
  connect(button, SIGNAL(triggered(bool)), this, SLOT(slotClose()));

  button = bar->actions().at(0)->menu()->actions().at(4);
  connect(button, SIGNAL(triggered(bool)), qApp, SLOT(quit()));      // exit application

}

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
  resize(1024, 768);

  QMenuBar* menu = createMenuBar(this);
  setMenuBar(menu);
  createConnections(menu);

  view.setModel(&SQLmodel);
}


