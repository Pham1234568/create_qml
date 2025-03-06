#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "Cuaso.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Cuaso window;
    window.show();

    return app.exec();
}
