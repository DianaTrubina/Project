#include <QString>
#include <QtTest>

#define private public // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! КАЛ
#include "../csvstream.h"

class Test_CsvStream : public QObject
{
    Q_OBJECT

  public:
    Test_CsvStream();

  private slots:
    void deleteExtraQuotes();
};

Test_CsvStream::Test_CsvStream()
{
}

void Test_CsvStream::deleteExtraQuotes()
{
  CsvStream csv("abc.csv");

  QString word1("\"one, two, three,\"\"\"\",\"\"\"\"\"");
  QString word1_r("one, two, three,\"\",\"\"");

  QString word2("\"\"");
  QString word2_r("");

  QCOMPARE(csv.deleteExtraQuotes(word1), word1_r);
  QCOMPARE(csv.deleteExtraQuotes(word2), word2_r);
  QCOMPARE(csv.deleteExtraQuotes(word2_r), word2_r);
}

QTEST_APPLESS_MAIN(Test_CsvStream)

#include "tst_test_csvstream.moc"
