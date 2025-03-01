#include <QApplication>
#include <QTranslator>
#include <QInputDialog>
#include <QStringList>
#include <QDebug>
#include "Cuaso.h"
#include <QDir>
#include <QCoreApplication>
#include<QFontDatabase>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    Cuaso w;
    w.show();
    a.setStyleSheet(" * { font-family: 'roboto'; font-weight: normal; color:#000;font-size: 12px}");

    return a.exec();
}
