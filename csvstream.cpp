#include "csvstream.h"

CsvStream::CsvStream(const QString &name, QObject *parent):file(name), QObject(parent)
{
}

QStringList CsvStream::readLine() // processRecord
{
  QString strRecordLine = createStrRecordLine(file);
  QStringList lstRecordLine = createLstRecordLine(strRecordLine);
  QString temp;

  /* этот фрагмент заключить в отдельную процедуру и засунуть в createLstRecordLine
   процедуру назвать что-то типо приведениеКВидуДляПредставления

  for (int i = 0; i < lstRecordLine.size(); ++i)    // идем по всем словам
  {
    temp = lstRecordLine.at(i);  // текущее слово

    if ((temp.at(0) == '\"') && (temp.at(temp.size()-1) == '\"'))
    {
      temp.remove(0, 1);
      temp.chop(1);
    }

    QString::iterator cur = temp.begin(); // первая буква слова

    int j = 0;
    while (cur != temp.end()) // идем по буквам слова
    {
      if ((*cur == '\"') && (*(cur+1) == '\"')) // т.к. символ " представляется в виде "",
        temp.remove(j+1, 1);                    // то удаляем дублирующую

      ++cur;
      ++j;
    }

    lstRecordLine.replace(i, temp);
  } */
}

QString CsvStream::createStrRecordLine(QFile& file) // возвращает string с одной записью из файла
{
  int quoteCount = 1;                   // количество " в строке. 1 - чтобы зайти в цикл
  QString strRecordLine;                // строка таблицы

  while (quoteCount % 2 != 0)           // кавычек нечетное кол-во (случай: xxx,"x \n x",xxx)
  {
    QString temp(file.readLine());      // считывает до первого \n
    temp.remove("\r\n");                // удаляем символ перехода на новую строку (в винде \r\n)

    if (temp.at(temp.size()-1) == '\n') // если символ был не \r\n, а просто \n, то удаляем его
      temp.chop(1);

    quoteCount = temp.count('\"');      // сколько раз встретилась "

    if (strRecordLine.isEmpty())        // агрегируем слова в одну строку-запись
      strRecordLine = temp;
    else
      strRecordLine += '\n' + temp;
  }

  return strRecordLine;
}

QStringList CsvStream::createLstRecordLine(const QString& strRecordLine) // возвращает list c записью, разбитой по полям
{
  QStringList lstRecordLine = strRecordLine.split(',');

  int quoteCount = 0;
  QStringList::iterator cur = lstRecordLine.begin();

  while (cur != lstRecordLine.end())      // пока не пройдем весь список
  {
    QString temp = *cur;                  // получили текущее слово
    quoteCount = temp.count('\"');        // сколько раз встретилась "

    if (quoteCount %2 != 0)               // кавычки не закрыты
    {
      cur->clear();
      (*cur) = temp + ',' + (*(cur+1));   // сливаем текущее слово со следующим
      lstRecordLine.erase(cur + 1);       // удаляем следующее, так как оно уже входит в состав текущего
    }
    else
      ++cur;  // если кавычки закрыты, то проверяем следующее слово
  }

  return lstRecordLine;
}
