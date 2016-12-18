#include <QApplication>
#include "graphwidget.h"


int main(int ac, char ** av)
{
    QApplication app(ac, av);
    GraphWidget form;
    form.setWindowFlags(Qt::FramelessWindowHint);
    form.move(0,0);
    form.resize(480,320);
    form.show();
    int exitCode = app.exec();
    return exitCode;
}
