#ifndef MAINENGINE_H
#define MAINENGINE_H

#include <QtSql>
#include <QFileInfo>
#include <QString>

#include "mytablemodel.h"
#include "csvstream.h"

class MainEngine:public QObject
{
  Q_OBJECT

  private:
    bool isOpen = false;

    QFileInfo currentFile;
    QSqlDatabase db;          // соединение с бд
    QSqlQueryModel model;
    MyTableModel csvModel;

  public:
    MainEngine(QObject* parent = 0);

    const MyTableModel& getConstLinkCsvModel() {return csvModel;}
    MyTableModel* getCsvModel() {return &csvModel;}
    QSqlQueryModel* getSqlModel() {return &model;}
    const QFileInfo& getCurrentFile() const {return currentFile;}
    bool isOpenNow() const {return isOpen;}

    void clear();
    void openSql(const QString& name);
    void openCsv(const QString& name);

    void convertToCsv(const QString& name);

  public slots:
    void fillSqlModel(const QString& name);

  signals:
    void sendListOfTables(const QStringList&);
};

#endif // MAINENGINE_H
