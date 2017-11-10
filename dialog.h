#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QtSql>
#include <QList>
#include <QString>

#include "mytablemodel.h"

class Dialog : public QDialog
{
  Q_OBJECT

  private:
    QSqlDatabase db;
    QStringList lst;

    QGroupBox* gbx1;
    QGroupBox* gbx2;
    QToolButton* cmd;
    QLineEdit* edit;
    QComboBox* combo;
    QDialogButtonBox* btns;

    QString whatTypeOfAttribute(const QString& str) const;
    void fillFromHeader(const QSqlQueryModel& model);
    void fillFromData(const QSqlQueryModel& model);

  public:
    Dialog(QWidget* parent = 0);

  public slots:
    void aqcuireDbName();
    void actWithDb(const QString& name);
    void convertToSQL();
};

#endif // DIALOG_H
