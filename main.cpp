#include <QtWidgets/QApplication>

#include "Interface/EntryWindow.h"


int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    EntryWindow w;
    w.show();
    return a.exec();
}