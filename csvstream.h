#ifndef CSVSTREAM_H
#define CSVSTREAM_H

#include <QDebug>

#include <QString>
#include <QStringList>
#include <QFile>

class CsvStream: public QObject
{
  Q_OBJECT

  private:
    QFile file;

    QString createStrRecordLine(); // !!!!!!!!!!!!!!!!!!!!!!!!!!!
    QStringList createLstRecordLine(const QString& strRecordLine); // !!!!!!!!!!!!!!!!!!!!!!!!!!
    QString deleteExtraQuotes(const QString& word); // !!!!!!!!!!!!!!!!!!!!!!!!!!

    QString prepareWordForCsv(const QString& word); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  public:
   // CsvStream():QObject(0) {file.setFileName("abc.csv");} // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    CsvStream(const QString& name, QObject *parent = 0);

    void open(QFile::OpenMode flags) {file.open(flags);}


    void close() {file.close();}
    bool atEnd() {return file.atEnd();}

    QStringList readLine();
    void writeLine(const QStringList& lstLine);


  //  QString deleteExtraQuotes(const QString& word); // !!!!!!!!!!!!!!!!!!!!!!!!!!
};

#endif // CSVSTREAM_H
