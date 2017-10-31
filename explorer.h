#ifndef EXPLORER_H
#define EXPLORER_H

#include <QtWidgets>
#include <QtSql>

class Explorer: public QWidget
{
  Q_OBJECT

  private:
    int flag;
    QFileSystemModel model;

    QGridLayout* layout = new QGridLayout(this);

    QPushButton* cmdBack = new QPushButton("Back", this);
    QLabel* lblPath = new QLabel(this);
    QTreeView* treeView = new QTreeView(this);
    QTableView* tableView = new QTableView(this);
    QSplitter* spl = new QSplitter(Qt::Horizontal, this);
    QLineEdit* edit = new QLineEdit(this);
    QPushButton* cmdOK = new QPushButton("OK", this);
    QPushButton* cmdCancel = new QPushButton("Cancel", this);

    void createConnections();

  public:
    Explorer(int k, QWidget* parent = 0);

    ~Explorer();

  signals:
    void transmitPath(const QString&);
    void transmitParent(const QModelIndex&);
    void openSql(const QString& path, const QString& name);
    void openCsv(const QString& path, const QString& name);

  private slots:
    void setLblPath(const QModelIndex& index);
    void rise(bool);
    void openAny();

};

#endif // EXPLORER_H
