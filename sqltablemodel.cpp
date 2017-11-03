#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(QObject* parent):QSqlTableModel(parent)
{
}

Qt::ItemFlags SqlTableModel::flags(const QModelIndex&) const
{
  return Qt::ItemIsSelectable;
}
