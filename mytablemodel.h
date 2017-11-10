#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QtWidgets>

class MyTableModel:public QAbstractTableModel
{
  private:
    int rows = 0;
    int columns = 0;

    QHash<QModelIndex, QString> storedData;
    QHash<int, QString> headers;

  public:
    MyTableModel(QObject* parent = 0);

    int rowCount(const QModelIndex& = QModelIndex()) const
    {
      return rows;
    }

    int columnCount(const QModelIndex& = QModelIndex()) const
    {
      return columns;
    }

    QVariant data(const QModelIndex& index, int role) const;   // геттер данных
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole); //сеттер одной ячейки
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);
    void clear();
    Qt::ItemFlags flags(const QModelIndex& index) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
};

#endif // MYTABLEMODEL_H
