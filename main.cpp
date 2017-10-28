#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QStringListModel model; //list будет изменено
    model.setStringList(QStringList() << "item1" << "item2" << "item3");

    MainWindow winWgt(0,&model); //list будет изменено

    winWgt.show();

    return app.exec();
}
