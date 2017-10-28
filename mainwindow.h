#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "mytablemodel.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget* parentObj, MyTableModel* model); //list будет изменено

    ~MainWindow();
};


#endif // MAINWINDOW_H
