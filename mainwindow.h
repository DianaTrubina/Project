#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget* parent, QStringListModel* model); //list будет изменено

    ~MainWindow();
};


#endif // MAINWINDOW_H
