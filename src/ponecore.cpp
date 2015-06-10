#include "ponecore.h"

PonyCore::PonyCore(QObject *parent)
    : QObject(parent)
{
    bgm = new PonyBGM();
}

PonyCore::~PonyCore()
{

}

void PonyCore::logMsg(QString str)
{
    cout << str << endl;
}

void PonyCore::TestThings()
{
    // Put any function or anything in here to test it
    bgm->PlayMusic("E:/Software_Dev/PoneNgine/compile/debug/resources/music/su_lvl_junglejoyride.ogg", 80);
}
