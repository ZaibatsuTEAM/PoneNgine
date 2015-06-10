#include <QApplication>
#include <QWidget>
#include "qogrewindow.h"
#include <iostream>
#include "ponesettings.h"
#include "ponecore.h"

PonyCore pne;

QTextStream cout(stdout);
QTextStream cin(stdin);

int argc = 0;

QApplication a(argc, (char**)0);

int main(int, char**)
{
    QOgreWindow* win = new QOgreWindow();

    a.processEvents();
    pne.logMsg("[INFO] Attempting to open Qt Ogre Window...");
    //pne.TestThings();
    win->show();

    return a.exec();
}
