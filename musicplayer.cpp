#include "musicplayer.h"


MusicPlayer* MusicPlayer::instance = nullptr;

MusicPlayer::MusicPlayer() {

    bgm = new QMediaPlayer;
    btnsound = new QMediaPlayer;
    clicksound = new QMediaPlayer;

    QAudioOutput *o1 = new QAudioOutput(bgm);
    QAudioOutput *o2 = new QAudioOutput(btnsound);
    QAudioOutput *o3 = new QAudioOutput(clicksound);

    bgm->setAudioOutput(o1);
    btnsound->setAudioOutput(o2);
    clicksound->setAudioOutput(o3);

    bgm->setSource(QUrl("qrc:/bgm/src/bgm/startSceneBGM.mp3"));
    btnsound->setSource(QUrl("qrc:/audio/src/sound/btnHoverSound.mp3"));
    clicksound->setSource(QUrl("qrc:/audio/src/sound/btnclick.mp3"));

    o1->setVolume(Settings::musicVol);
    o2->setVolume(Settings::soundVol);
    o3->setVolume(Settings::soundVol);

    bgm->setLoops(QMediaPlayer::Infinite);
}

MusicPlayer::~MusicPlayer()
{
    delete bgm;
    delete btnsound;
}

void MusicPlayer::startBGM()
{
    bgm->setLoops(QMediaPlayer::Infinite);
    bgm->play();

}

void MusicPlayer::startBtnSound()
{
    btnsound->setPosition(0);
    btnsound->play();
}

void MusicPlayer::clickSound()
{
    clicksound->setPosition(0);
    clicksound->play();
}

QMediaPlayer *MusicPlayer::getBgmPlayer()
{
    return bgm;
}

