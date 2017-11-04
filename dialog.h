#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QtSql>

class Dialog : public QDialog
{
  Q_OBJECT

  private:
    QSqlDatabase db;
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
};

#endif // DIALOG_H
