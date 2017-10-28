#include "mainwindow.h"

MainWindow::MainWindow(QWidget* parent, QStringListModel* model):QMainWindow(parent) //list будет изменено
{
  QMenuBar* menu = new QMenuBar(this);

  QMenu* menuFile = new QMenu("File", menu);
  QMenu* menuConvert = new QMenu("Convert to ...", menuFile);

  menu->addAction(menuFile->menuAction());

  menuFile->addAction("Open");
  menuFile->addAction("Save");
  menuFile->addAction(menuConvert->menuAction());
  menuFile->addAction("Close");
  menuFile->addSeparator();
  menuFile->addAction("Exit",qApp,SLOT(quit())); //повисает. пока не знаю, как писать правильно.

  menuConvert->addAction("csv");
  menuConvert->addAction("sqlite");

  setMenuBar(menu);

  QTableView* table = new QTableView(this);
  table->setModel(model);

  setCentralWidget(table);

}

MainWindow::~MainWindow()
{
  // ???????????????????????????????????????
  // а как вызвать деструктор предка? а надо ли вообще, и так само удалится при Exit.

  delete this; //?????????
}
