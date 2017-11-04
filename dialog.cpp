#include "dialog.h"

Dialog::Dialog(QWidget *parent):QDialog(parent)
{
  setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint);
  setWindowTitle("Convert to SQLite");

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

  connect(cmd, SIGNAL(clicked(bool)), this, SLOT(getDbName()));
  connect(edit, SIGNAL(textChanged(const QString&)), this, SLOT(actWithDb(const QString&)));

  // соединить OK.clicked c convertToSql
  // Cancel.clicke c rejected
}

void Dialog::getDbName()
{
  // надо будет заменить кнопки "Save" на "Open"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  QString dbName = QFileDialog::getSaveFileName(this, "Explorer", "", "SQLite files(*.sqlite *.db)", Q_NULLPTR, QFileDialog::DontConfirmOverwrite);

  if(dbName != "") // если реально был выбран файл, а не нажали Cancel
    edit->setText(dbName);
}

void Dialog::actWithDb(const QString& name)
{
  combo->setEnabled(true);

  db = QSqlDatabase::addDatabase("QSQLITE", "tempDbConnection");
  db.setDatabaseName(name);
  db.open(); // открывает существующую либо создает новую

  lst = db.tables();

  if (! lst.empty())        // если в базе есть таблицы
    combo->addItems(lst);
  else                      // база пустая
  {
      // ставим в текущий текст комбо-бокса имя открытого в mainwindow файла
  }
}

void Dialog::convertToSQL()
{
  if ((! lst.empty()) && db.tables().toSet().contains(combo->currentText())) // база не пуста, таблица уже есть
  {
    QSqlQuery query(db);
    query.exec("DROP TABLE " + combo->currentText() + ";");
  }

  QSqlQuery query(db);
  QString str = "CREATE TABLE " + combo->currentText() + " ( ";
  // в цикле пройти по горизонтальным заголовкам и первой строке модели, определяя имя и тип

  // вызов accepted()

}
