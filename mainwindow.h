#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>        //qtableview

#include <QtSql>
#include "sqltablemodel.h"
#include "dialog.h"

class MainWindow: public QMainWindow
{
  Q_OBJECT

  private:
    bool isOpen = false;

    QSqlDatabase db;          // соединение с бд
    QSqlQueryModel model;

    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTableView* view;
    QComboBox* tablesBox;
    Dialog* dialog;

    void createMenuBar(MainWindow* parent);
    void createToolBar(MainWindow* parent);
    void makeEnabled(const QString& str);
    void makeDisabled();
    void createConnections();
    void openSql(const QString& name);
    void openCsv(const QString& name);

  public:
    MainWindow(QWidget* parent = 0);

    const QSqlQueryModel& getModel()
    {
      return model;
    }

  public slots:
    void slotOpen();        // открыть проводник
    void setModelForTable(const QString& name);
    void convertToSql();
};


#endif // MAINWINDOW_H
