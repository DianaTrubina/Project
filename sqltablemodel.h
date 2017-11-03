#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QtSql>

class SqlTableModel: public QSqlTableModel
{
  Q_OBJECT

  public:
    SqlTableModel(QObject* parent = 0);

    Qt::ItemFlags flags(const QModelIndex&) const override;
};

#endif // SQLTABLEMODEL_H
