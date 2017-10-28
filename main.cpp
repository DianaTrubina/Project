#include "mainwindow.h"
#include "mytablemodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyTableModel model;

    // заполнение модели

    MainWindow winWgt(0,&model);

    winWgt.show();

    return app.exec();
}
