#include "dialog.h"
#include <QString>
#include "mainwindow.h"

Dialog::Dialog(QWidget* parent):QDialog(parent)
{
  setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint);
  setWindowTitle("Convert to SQLite");

  setAttribute(Qt::WA_DeleteOnClose); // поэтому не буду писать отсоединение базы и деструктор

  gbx1 = new QGroupBox(this);
  gbx2 = new QGroupBox(this);
  btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  cmd = new QToolButton(gbx1);
  edit = new QLineEdit(gbx1);
  combo = new QComboBox(gbx2);

  gbx1->setTitle("Choose db");
  gbx2->setTitle("Choose table");
  cmd->setText("...");
  edit->setReadOnly(true);
  combo->setEditable(true);
  combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  combo->setInsertPolicy(QComboBox::NoInsert);
  combo->setEnabled(false);

  QVBoxLayout* vbx = new QVBoxLayout(this);
  vbx->addWidget(gbx1);
  vbx->addWidget(gbx2);
  vbx->addWidget(btns);
  this->setLayout(vbx);

  QHBoxLayout* hbx1 = new QHBoxLayout(gbx1);
  hbx1->addWidget(edit);
  hbx1->addWidget(cmd);
  gbx1->setLayout(hbx1);

  QHBoxLayout* hbx2 = new QHBoxLayout(gbx2);
  hbx2->addWidget(combo);
  gbx2->setLayout(hbx2);

  connect(cmd, SIGNAL(clicked(bool)), this, SLOT(aqcuireDbName()));
  connect(edit, SIGNAL(textChanged(const QString&)), this, SLOT(actWithDb(const QString&)));
  connect(btns, SIGNAL(accepted()), this, SLOT(convertToSQL()));
  connect(btns, SIGNAL(rejected()), this, SLOT(reject()));
}

void Dialog::aqcuireDbName()
{
  // надо будет заменить кнопки "Save" на "Open"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  QString dbName = QFileDialog::getSaveFileName(this, "Explorer", "", "SQLite files(*.sqlite *.db)", Q_NULLPTR, QFileDialog::DontConfirmOverwrite);

  if(dbName != "")          // если реально был выбран файл, а не нажали Cancel
    edit->setText(dbName);
}

void Dialog::actWithDb(const QString& name)
{
  db = QSqlDatabase::addDatabase("QSQLITE", "tempDbConnection");
  db.setDatabaseName(name);
  db.open();                // открывает существующую либо создает новую

  combo->setEnabled(true);
  lst = db.tables();

  if (! lst.empty())        // если в базе есть таблицы
    combo->addItems(lst);
  else                      // база пустая
  {
      combo->addItem(((MainWindow*) parent())->whatFileName());
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
}

void Dialog::convertToSQL()
{
  if ((! lst.empty()) && db.tables().toSet().contains(combo->currentText())) // база не пуста, таблица уже есть
  {
    QSqlQuery query(db);
    query.exec("DROP TABLE " + combo->currentText() + ";");
  }

  const QSqlQueryModel& model = ((MainWindow*) parent())->getModel();

  fillHeader(model);

  if (model.rowCount())
  {
    fillData(model);
  }

  accept(); // вызов accept(), т.к. нажимали OK
}

QString Dialog::whatTypeOfAttribute(const QString& str) const
{
  QRegExp reg("^\\-?\\d+\\.?\\d+$");

  if (str.contains(reg))
     return "REAL";

  reg = QRegExp("^\\-?\\d+$");
  if(str.contains(reg))
    return "INTEGER";

  return "TEXT";
}

void Dialog::fillHeader(const QSqlQueryModel& model)
{
  QSqlQuery query(db);
  QString str = "CREATE TABLE " + combo->currentText() + " (";

  for (int i = 0; i < model.columnCount(); i++)
  {
    str += model.headerData(i, Qt::Horizontal).toString() + ' ';

    if (model.rowCount()) // если в модели есть данные, а не только заголовок
      str += whatTypeOfAttribute(model.data(model.index(0, i)).toString()); // тип поля
    else
      str += "TEXT";      // если модель пуста, то в базе все поля будут TEXT

    if (i != model.columnCount() - 1)
      str += ", ";
  }

  str += ");";
  query.exec(str);
}

void Dialog::fillData(const QSqlQueryModel& model)
{
  QString str;
  QSqlQuery query(db);

  db.transaction();

  for (int i = 0; i < model.rowCount(); i++)
  {
    str = "INSERT INTO " + combo->currentText() + " VALUES (";

    for (int j = 0; j < model.columnCount(); j++)
    {
      str += model.data(model.index(i, j)).toString(); // вставляем как строку, база сама преобразует

      if (j != model.columnCount() - 1)
        str += ", ";
    }

    str += ");";
    query.exec(str);
  }

  db.commit();
}
