#include "mainwindow.h"
#include "rateswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RatesPage m;

    return a.exec();
}
