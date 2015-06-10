#include "PoneSound.h"

PonyBGM::PonyBGM(QObject *parent)
    : QObject(parent)
{

}

PonyBGM::~PonyBGM()
{
    delete vinylScratch;
}

PonySFX::PonySFX(QObject *parent)
    : QObject(parent)
{

}

PonySFX::~PonySFX()
{
    delete octaviaMelody;
}

void PonyBGM::PlayMusic(QString file, int vol, bool loop)
{
    vinylScratch->setMedia(QUrl::fromLocalFile(file));
    vinylScratch->setVolume(vol);
    if (!loop) vinylScratch->play();

    while (loop)
    {
        if (vinylScratch->state() == PonyMusicPlayer::StoppedState)
        {
            vinylScratch->play();
        }
    }
}

void PonySFX::PlaySound(QString file, PReal_D vol, int loopCount, bool loop)
{
    octaviaMelody->setSource(QUrl::fromLocalFile(file));
    octaviaMelody->setVolume(vol);

    if (loop) octaviaMelody->setLoopCount(PonySoundEffect::Infinite); // loopCount = -2;
    else      octaviaMelody->setLoopCount(loopCount);
}
