#ifndef EXPLORER_H
#define EXPLORER_H

#include <QtWidgets>

class Explorer: public QWidget
{
  Q_OBJECT

  private:
    QFileSystemModel model;

    QTreeView* treeView = new QTreeView(this);
    QTableView* tableView = new QTableView(this);
    QSplitter* spl = new QSplitter(Qt::Horizontal, this);
    QVBoxLayout* layout = new QVBoxLayout(this);

  public:
    Explorer(QWidget* parent = 0);

    ~Explorer();

};

#endif // EXPLORER_H
