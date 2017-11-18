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



    void handleWordToCsv(QFile &file, QString word); // перенести в CsvEngine


  public:
    MainEngine(QObject* parent = 0);

    const MyTableModel& whatCsvModel() {return csvModel;} // скорее всего совсем уберу
    MyTableModel* getCsvModel() {return &csvModel;}
    QSqlQueryModel* getSqlModel() {return &model;}
    QString getCurrentFileName() const {return currentFile.baseName();}
    bool isOpenNow() const {return isOpen;}

    void clear();
    void openSql(const QString& name);
    void openCsv(const QString& name);

  public slots:
    void fillSqlModel(const QString& name);

  signals:
    void sendListOfTables(const QStringList&);
};

#endif // MAINENGINE_H
