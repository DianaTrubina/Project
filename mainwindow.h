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

    QMenuBar* createMenuBar(MainWindow* parent);

    void createConnections(QMenuBar* bar);

  public:
    MainWindow(QWidget* parent = 0);

    QSqlTableModel* getSqlModel()
    {
      QSqlTableModel* p = &SQLmodel;

      return p;
    }

  public slots:
    void slotOpen();                                           // открыть проводник
    void slotClose();                                          // очистить модель и представление
    void openSql(const QString& path, const QString& name);
    void openCsv(const QString& path, const QString& name);
};


#endif // MAINWINDOW_H
