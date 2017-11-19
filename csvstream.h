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
    QStringList createLstRecordLine(const QString& strRecordLine); // OK
    QString deleteExtraQuotes(const QString& word); // OK

    QString prepareWordForCsv(const QString& word); // OK

  public:
    CsvStream(const QString& name, QObject *parent = 0);

    void open(QFile::OpenMode flags) {file.open(flags);}
    void close() {file.close();}
    bool atEnd() {return file.atEnd();}

    QStringList readLine();
    void writeLine(const QStringList& lstLine);
};

#endif // CSVSTREAM_H
