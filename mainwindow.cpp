#include "mainwindow.h"
#include <QDebug>

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
}

void MainWindow::slotClose()
{
}

void MainWindow::createConnections(QMenuBar* bar)
{
  QAction* button = bar->actions().at(0)->menu()->actions().at(0);   // open
  connect(button, SIGNAL(triggered(bool)), this, SLOT(slotOpen()));

  button = bar->actions().at(0)->menu()->actions().at(2);            // close
  connect(button, SIGNAL(triggered(bool)), this, SLOT(slotClose()));

  button = bar->actions().at(0)->menu()->actions().at(4);
  connect(button, SIGNAL(triggered(bool)), qApp, SLOT(quit()));      // exit application
}

MainWindow::MainWindow(QWidget* parent):QMainWindow(parent)
{
  QMenuBar* menu = createMenuBar(this);
  setMenuBar(menu);
  createConnections(menu);

  view.setModel(&SQLmodel);

  Explorer* dialog = new Explorer(this);

  dialog->show();
}

MainWindow::~MainWindow()
{
  // а как вызвать деструктор предка QMainWindow?
  delete this; //?????????
}
