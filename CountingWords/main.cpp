#include <QApplication>
#include "CountingWordMWindow.h"

int main(int argc, char *argv[])
    {
    QApplication a(argc, argv);
    CountingWordMWindow w;
    w.show();
    return a.exec();
    }
