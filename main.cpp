#include "MultiGDB.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MultiGDB w;
    w.show();
    return a.exec();
}
