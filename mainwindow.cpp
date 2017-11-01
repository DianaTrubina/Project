#include "mainwindow.h"
#include <QFileDialog>

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

  menuConvert->addAction("csv");                    // 1.0
  menuConvert->addAction("sqlite");                 // 1.1

  menuBar->actions().at(0)->menu()->actions().at(1)->setEnabled(false);
  menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(0)->setEnabled(false);
  menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(1)->setEnabled(false);
}

void MainWindow::makeEnabled(const QString& str)
{
  menuBar->actions().at(0)->menu()->actions().at(1)->setEnabled(true);

  if (str == "sql")
    menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(0)->setEnabled(true);
  else
    menuBar->actions().at(0)->menu()->actions().at(1)->menu()->actions().at(1)->setEnabled(true);
}

void MainWindow::slotOpen()
{
  QString name = QFileDialog::getOpenFileName(this, "Explorer", "", "SQLite files(*.sqlite);;CSV files(*.csv)");

  if (name.endsWith(".sqlite3"))
  {
    openSql(name);
    makeEnabled("sql");
  }
  else
    if (name.endsWith(".csv"))
    {
      openCsv(name);
      makeEnabled("csv");
    }
}

void MainWindow::openCsv(const QString& name)
{
}

void MainWindow::openSql(const QString& name)
{
 // SQLmodel.setTable(name); // просто имя или с путем?
 // SQLmodel.select();
}

void MainWindow::createConnections()
{
  QAction* button = menuBar->actions().at(0)->menu()->actions().at(0);   // open
  connect(button, SIGNAL(triggered(bool)), this, SLOT(slotOpen()));

  // connect для convert

  button = menuBar->actions().at(0)->menu()->actions().at(3);
  connect(button, SIGNAL(triggered(bool)), qApp, SLOT(quit()));      // exit application

}

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
  resize(1024, 768);

  createMenuBar(this);
  setMenuBar(menuBar);

  createConnections();

  view.setModel(&SQLmodel);
}


