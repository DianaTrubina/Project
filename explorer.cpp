#include "explorer.h"

Explorer::Explorer(QWidget* parent):QWidget(parent, Qt::Window | Qt::WindowStaysOnTopHint)
{
  model.setRootPath(QDir::rootPath());

  treeView->setModel(&model);
  tableView->setModel(&model);

  spl->addWidget(treeView);
  spl->addWidget(tableView);



  layout->addWidget(spl);
  // layout->кнопки и т.д.

  setLayout(layout);

  connect(treeView, SIGNAL(clicked(const QModelIndex&)), tableView, SLOT(setRootIndex(const QModelIndex&)));
  connect(tableView, SIGNAL(activated(const QModelIndex&)), treeView, SLOT(setCurrentIndex(const QModelIndex&)));
  connect(tableView, SIGNAL(activated(const QModelIndex&)), tableView, SLOT(setRootIndex(const QModelIndex&)));
}

Explorer::~Explorer()
{
  delete this;
}
