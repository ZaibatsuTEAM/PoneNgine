#ifndef PONYSOUND_H
#define PONYSOUND_H

#include <ponesettings.h>

#include <QObject>
#include <QMediaPlayer>
#include <QSoundEffect>

typedef QSoundEffect PonySoundEffect;
typedef QMediaPlayer PonyMusicPlayer;

class PonyBGM : public QObject
{
    Q_OBJECT
public:
    explicit PonyBGM(QObject *parent = 0);
    ~PonyBGM();

    PonyMusicPlayer* vinylScratch;
signals:

public slots:
    void PlayMusic(QString file, int vol, bool loop = false);

};

class PonySFX : public QObject
{
    Q_OBJECT
public:
    explicit PonySFX(QObject *parent = 0);
    ~PonySFX();

    PonySoundEffect* octaviaMelody;

public slots:
    void PlaySound(QString file, PReal_D vol, int loopCount = 0, bool loop = false);
};

#endif // PONYSOUND_H
