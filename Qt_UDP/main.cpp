#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w,w1;

    w.show();

    w1.show();

    return a.exec();
}
