#ifndef CSVSTREAM_H
#define CSVSTREAM_H

#include <QString>
#include <QStringList>
#include <QFile>

class CsvStream: public QObject
{
  Q_OBJECT

  private:
    QFile file;

    QString createStrRecordLine(QFile& file); // !!!!!!!!!!!!!!!!!!!!!!!!!!!
    QStringList createLstRecordLine(const QString& strRecordLine); // !!!!!!!!!!!!!!!!!!!!!!!!!!

  public:
    CsvStream(const QString& name, QObject *parent = 0);

    void close() {file.close();}
    QStringList readLine(); // processRecord

};

#endif // CSVSTREAM_H
