#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>        //qtableview
#include <QtSql>
#include <QFileInfo>
#include <QFile>
#include <QStringList>
#include <QString>

#include "mytablemodel.h"

class Dialog;

class MainWindow: public QMainWindow
{
  Q_OBJECT

  private:
    bool isOpen = false;

    QSqlDatabase db;          // соединение с бд
    QSqlQueryModel model;
    MyTableModel csvModel;

    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTableView* view;
    QComboBox* tablesBox;
    Dialog* dialog;

    QFileInfo currentFile;

    void createMenuBar(MainWindow* parent);
    void createToolBar(MainWindow* parent);
    void makeEnabled(const QString& str);
    void makeDisabled();
    void createConnections();
    void openSql(const QString& name);
    void openCsv(const QString& name);
    void processRecord(QFile& file, QStringList& lstheaders);

  public:
    MainWindow(QWidget* parent = 0);

    QFileInfo fgfg;

    const MyTableModel& getModel() // тип модели !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
      return csvModel;
    }

    QString whatFileName() const
    {
      return currentFile.baseName();
    }

  public slots:
    void slotOpen();                               // открыть проводник
    void setModelForTable(const QString& name);
    void convertToSql();
};


#endif // MAINWINDOW_H
