#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>        //qtableview
#include <QString>

#include "mainengine.h"
#include "dialog.h"

class Dialog;

class MainWindow: public QMainWindow
{
  Q_OBJECT

  private:
    MainEngine guts;

    QMenuBar* menuBar;
    QToolBar* toolBar;
    QTableView* view;
    QComboBox* tablesBox;
    Dialog* dialog;

    void createMenuBar(MainWindow* parent); // OK
    void createToolBar(MainWindow* parent); // OK
    void makeEnabled(const QString& fileType); // OK     fileType: csv либо sql
    void makeDisabled(); // OK
    void createConnections();

  public:
    MainWindow(QWidget* parent = 0);

    const QFileInfo& whatCurrentFile() {return guts.getCurrentFile();}
    const MyTableModel& getConstCsvModel() {return guts.getConstLinkCsvModel();}

  public slots:
    void slotOpen();                               // открыть проводник
    void slotFillBox(const QStringList& tables);

    void convertToSql();
    void convertToCsv();
};


#endif // MAINWINDOW_H
