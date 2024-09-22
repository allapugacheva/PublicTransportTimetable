#include "mainwindow.h"
#include "citywindow.h"
#include "routeswindow.h"
#include "routestopswindow.h"
#include "routestoptimetablewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cityWindow w;       // Создание окна.
    w.show();           // Вызов окна.
    return a.exec();
}
