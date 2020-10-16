#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat fmt;

    fmt.setProfile(QSurfaceFormat::CoreProfile);
    //fmt.setSamples(16);
    QSurfaceFormat::setDefaultFormat(fmt);

    MainWindow w;
    w.show();

    return a.exec();
}
