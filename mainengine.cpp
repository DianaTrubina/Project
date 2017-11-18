#include "mainengine.h"

MainEngine::MainEngine(QObject* parent):QObject(parent)
{
}

void MainEngine::fillSqlModel(const QString& name)
{
  if (!(name == "")) // чтобы не было реакции при очистке через makeDisabled()
  {
    QSqlQuery query(db);
    query.exec("SELECT * FROM " + name + ";");

    model.setQuery(query);
  }
}

void MainEngine::clear()
{
  model.clear();      // чистим модель и, следовательно, представление
  csvModel.clear();

  if (db.isOpen())    // если в прошлый Open открывалась база
    db.close();

  isOpen = false;     // сбрасываем состояние
}

void MainEngine::openSql(const QString& name)
{
  isOpen = true;
  currentFile = QFileInfo(name);

  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(name);
  db.open();

  emit sendListOfTables(db.tables());
}

void MainEngine::openCsv(const QString& name)
{
  isOpen = true;
  currentFile = QFileInfo(name);

  QFile file(name);             // !!!!!!!!!!!!!!!!!!!!!   заменится на CsvStream
  file.open(QIODevice::ReadOnly);// !!!!!!!!!!!!!!!!!!!!!!

  QStringList fields;               // в первом случае - заголовки столбцов, во втором - поля записи
  processRecord(file, fields);// !!!!!!!!!!!!!!!!!!!!!!!!!!!
  csvModel.insertColumns(0, fields.size());

  for (int j = 0; j < fields.size(); ++j)
    csvModel.setHeaderData(j, Qt::Horizontal, fields.at(j));

  int rows = 0;
  while (!file.atEnd()) // если в файле была только строка шапки, то сюда даже не зайдем
  {
    processRecord(file, fields); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    csvModel.insertRows(rows, 1);

    for (int j = 0; j < fields.size(); ++j)
      csvModel.setData(csvModel.index(rows, j), fields.at(j));

    ++rows;
  }

  file.close(); // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}






