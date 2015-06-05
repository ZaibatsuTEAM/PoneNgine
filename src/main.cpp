#include <QApplication>
#include <QWidget>
#include "qogrewindow.h"
#include <iostream>

QTextStream out(stdout);
//QTextStream in(stdin);

int argc = 0;

QApplication a(argc, (char**)0);

int main(int, char**)
{
    QOgreWindow* win = new QOgreWindow();

    a.processEvents();
    out << "Attempting to open Qt Ogre Window..." << endl;
    win->show();

    return a.exec();
}
