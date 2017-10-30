#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>        //qtableview
#include <QtSql>

#include "explorer.h"

class MainWindow: public QMainWindow
{
  Q_OBJECT

  private:
    QTableView view;
    QSqlTableModel SQLmodel;  // модель для данных, возвращенных sql-запросами
   // QAbstractTableModel       // модель для данных из csv

    QMenuBar* createMenuBar(MainWindow* parent);
    void createConnections(QMenuBar* bar);

  public:
    MainWindow(QWidget* parent = 0);

    ~MainWindow();

  public slots:
    void slotOpen();      // открыть новую базу или csv
    void slotClose();     // очистить модель и представление

};


#endif // MAINWINDOW_H
