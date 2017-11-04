#include "dialog.h"

Dialog::Dialog(QWidget *parent):QDialog(parent)
{
  setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint);
  setWindowTitle("Convert to CSV");

  gbx1 = new QGroupBox;
  gbx2 = new QGroupBox;
  btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  cmd = new QToolButton;
  edit = new QLineEdit;
  combo = new QComboBox;

  gbx1->setTitle("Choose db");
  gbx2->setTitle("Choose table");
  gbx2->setEnabled(false);
  cmd->setText("...");
  edit->setReadOnly(true);
  combo->setEditable(true);
  combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  combo->setInsertPolicy(QComboBox::NoInsert);

  QVBoxLayout* vbx = new QVBoxLayout;
  vbx->addWidget(gbx1);
  vbx->addWidget(gbx2);
  vbx->addWidget(btns);
  this->setLayout(vbx);

  QHBoxLayout* hbx = new QHBoxLayout;
  hbx->addWidget(edit);
  hbx->addWidget(cmd);
  gbx1->setLayout(hbx);

  connect(cmd, SIGNAL(clicked(bool)), this, SLOT(getDbName()));
  connect(edit, SIGNAL(textChanged(const QString&)), this, SLOT(actWithDb(const QString&)));
}

void Dialog::getDbName()
{
  // надо будет заменить кнопки "Save" на "Open"
  QString dbName = QFileDialog::getSaveFileName(this, "Explorer", "", "SQLite files(*.sqlite *.db)", Q_NULLPTR, QFileDialog::DontConfirmOverwrite);

  if(dbName != "") // если реально был выбран файл, а не нажали Cancel
  {
    edit->setText(dbName);
  }
}

void Dialog::actWithDb(const QString& name)
{
  db = QSqlDatabase::addDatabase("QSQLITE", "tempDbConnection");
  db.setDatabaseName(name);
  db.open(); // открывает существующую либо создает новую

  QStringList lst = db.tables();

   if (!lst.empty())
   {
      gbx2->setEnabled(true);
      combo->addItems(lst);
      //... пользователь либо выбирает имеющуюся, либо вводит одну, которая в mainwindow.cpp добавляется в список
   }
}
