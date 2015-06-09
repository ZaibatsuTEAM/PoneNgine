#include "ponecore.h"

PonyCore::PonyCore(QObject *parent)
    : QObject(parent)
{

}

PonyCore::~PonyCore()
{

}

void PonyCore::logMsg(QString str)
{
    cout << str << endl;
}
