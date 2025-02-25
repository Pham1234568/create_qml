#include <QApplication>
#include <QTranslator>
#include "Cuaso.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    if (translator.load("D:/Code_Qt/Code_Qt/Code_Qt_fr.qm")) {
        a.installTranslator(&translator);
    }

    Cuaso w;
    w.show();

    return a.exec();
}
