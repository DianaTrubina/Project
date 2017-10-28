#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QtWidgets>

class MyTableModel:public QAbstractTableModel
{
  private:
    int rows;
    int columns;
    QHash<QModelIndex, QVariant> storedData;

  public:
    MyTableModel(int rCount, int cCount, QObject* parentObj = 0);

    QVariant data(const QModelIndex& index, int role) const;   // геттер данных

    bool setData(const QModelIndex& index, const QVariant& value, int role); //сеттер одной ячейки

    int rowCount(const QModelIndex&) const;

    int columnCount(const QModelIndex&) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex& index) const;
};

#endif // MYTABLEMODEL_H
