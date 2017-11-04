#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QtSql>
#include <QList>

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

  public:
    Dialog(QWidget* parent = 0);

  public slots:
    void getDbName();
    void actWithDb(const QString& name);
    void convertToSQL();
};

#endif // DIALOG_H
