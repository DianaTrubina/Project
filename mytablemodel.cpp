#include "mytablemodel.h"

MyTableModel::MyTableModel(int rCount, int cCount, QObject* parentObj):QAbstractTableModel(parentObj),
                                                                       rows(rCount),
                                                                       columns(cCount)
{
}

QVariant MyTableModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid())
    return QVariant();
  else
    return (role == Qt::DisplayRole || role == Qt::EditRole) ? storedData.value(index) : QVariant();
}

bool MyTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (index.isValid() && role == Qt::EditRole)
  {
    storedData[index] = value;
    emit dataChanged(index, index);
    return true;
  }
  else
    return false;
}

int MyTableModel::rowCount(const QModelIndex&) const
{
  return rows;
}

int MyTableModel::columnCount(const QModelIndex&) const
{
  return columns;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// если другие параметры - то уже не перезаписанная, а значит не будет использоваться
// как передать названия столбцов? сделать поле в private?
QVariant MyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role !=Qt::DisplayRole)
    return QVariant();
  else
    return (orientation == Qt::Horizontal) ? /*соотв. название*/ : QString::number(section);
}

Qt::ItemFlags MyTableModel::flags(const QModelIndex& index) const
{
  Qt::ItemFlags flag = QAbstractTableModel::flags(index);

  return index.isValid() ? (flag | Qt::ItemIsEditable) : flag;
}
