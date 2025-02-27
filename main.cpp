#include <QApplication>
#include <QTranslator>
#include <QInputDialog>
#include <QStringList>
#include <QDebug>
#include "Cuaso.h"
#include <QDir>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);




    Cuaso w;
    w.show();
    return a.exec();
}
