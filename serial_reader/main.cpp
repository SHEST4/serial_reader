#include "mainwindow.h"

#include <QApplication>

void find_and_print_ports();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
