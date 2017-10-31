#include "explorer.h"

void Explorer::setLblPath(const QModelIndex& index)
{
  QString path = model.filePath(index);

  emit transmitPath(path);
}

void Explorer::rise(bool)
{
  const QModelIndex& index = treeView->currentIndex().parent();

  emit transmitParent(index);
}

void Explorer::openAny()
{
  // сохраняем последние записи в полях

  QString path = lblPath->text();
  QString name = edit->text();

  QRegExp reg("\\.sqlite3");

  if (name.contains(reg))
    emit openSql(path, name);
  else
    emit openCsv(path, name);

  close();
}

void Explorer::createConnections()
{
  // чем rootindex отличается от current? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  connect(treeView, SIGNAL(clicked(const QModelIndex&)), tableView, SLOT(setRootIndex(const QModelIndex&)));
  connect(tableView, SIGNAL(activated(const QModelIndex&)), treeView, SLOT(setCurrentIndex(const QModelIndex&)));
  connect(tableView, SIGNAL(activated(const QModelIndex&)), tableView, SLOT(setRootIndex(const QModelIndex&)));

  // установка пути в метке lbl
  connect(treeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(setLblPath(const QModelIndex&)));
  connect(tableView, SIGNAL(activated(const QModelIndex&)), this, SLOT(setLblPath(const QModelIndex&)));
  connect(this, SIGNAL(transmitPath(const QString&)), lblPath, SLOT(setText(const QString&)));

  // подняться на уровень выше в иерархии файловой системы
  connect(cmdBack, SIGNAL(clicked(bool)), this, SLOT(rise(bool)));
  connect(this, SIGNAL(transmitParent(const QModelIndex&)), treeView, SLOT(setCurrentIndex(const QModelIndex&)));
  connect(this, SIGNAL(transmitParent(const QModelIndex&)), tableView, SLOT(setRootIndex(const QModelIndex&)));
  connect(this, SIGNAL(transmitParent(const QModelIndex&)), this, SLOT(setLblPath(const QModelIndex&)));

  // не описал подстановку имени файла в поле ввода

  connect(cmdCancel, SIGNAL(clicked(bool)), this, SLOT(close()));

  switch(flag)
  {
    case 1: // открытие sqlite или csv
      connect(cmdOK, SIGNAL(clicked(bool)), this, SLOT(openAny()));
      break;
    case 2:
      //c
      break;
    case 3:
      //c
      break;
  }

}

Explorer::Explorer(int k, QWidget* parent):QWidget(parent, Qt::Window | Qt::WindowStaysOnTopHint), flag(k)
{
  setAttribute(Qt::WA_DeleteOnClose);

  model.setRootPath(QDir::rootPath());

  treeView->setModel(&model);
  tableView->setModel(&model);

  spl->addWidget(treeView);
  spl->addWidget(tableView);

  layout->addWidget(cmdBack, 0, 0, 1, 1);
  layout->addWidget(lblPath, 0, 1, 1, 4);
  layout->addWidget(spl, 1, 0, 4, 5);
  layout->addWidget(edit, 5, 0, 1, 3);
  layout->addWidget(cmdOK, 5, 3, 1, 1);
  layout->addWidget(cmdCancel, 5, 4, 1, 1);
  // layout->кнопки и т.д.

  setLayout(layout);
  setWindowTitle("Explorer");
  resize(800, 600);

  createConnections();
}

Explorer::~Explorer()
{
  delete this;
}
