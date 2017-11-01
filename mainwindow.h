#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>        //qtableview
#include <QtSql>

class MainWindow: public QMainWindow
{
  Q_OBJECT

  private:
    QTableView view;
    QSqlTableModel SQLmodel;  // модель для данных, возвращенных sql-запросами
    QMenuBar* menuBar;
    // QSqlDatabase

    void createMenuBar(MainWindow* parent);
    void createConnections();
    void openSql(const QString& name);
    void openCsv(const QString& name);

  public:
    MainWindow(QWidget* parent = 0);

  signals:
    void requestToBeEnabled(const QString& type);

  public slots:
    void slotOpen();                                           // открыть проводник
    void makeEnabled(const QString& str);
};


#endif // MAINWINDOW_H
