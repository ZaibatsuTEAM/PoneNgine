#ifndef PONECORE_H
#define PONECORE_H

#include <QObject>
#include <QTextStream>
#include <QString>

#include "ponesettings.h"
#include "Sound/PoneSound.h"

class PonyCore : public QObject
{
    Q_OBJECT
public:
    explicit PonyCore(QObject *parent = 0);
    ~PonyCore();

    void logMsg(QString str);

    void TestThings();

private:
    PonyBGM* bgm;
};

extern QTextStream cout;
extern QTextStream cin;
extern PonyCore pne;

#endif // PONECORE_H
