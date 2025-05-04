#include "musicplayer.h"
#include "animation.h"


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

    QObject::connect(bgm,&QMediaPlayer::positionChanged,[this](int pos){
        if(pos == 0){
            bgm->setPosition(0);
            bgm->play();
        }
    });

    /* debug
     *
     * qDebug() << "musicvol: " << o1->volume() << "expect: " << Settings::musicVol;
     * qDebug() << "soundvol: " << o2->volume() << ',' << o3->volume() << "expect: " << Settings::soundVol;
    */
}

MusicPlayer::~MusicPlayer()
{
    delete bgm;
    delete btnsound;
}

void MusicPlayer::startBGM()
{
    bgm->play();
    // qDebug() << "bgm start playing. loop:" << bgm->loops() << " expect:" << QMediaPlayer::Infinite;
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

void MusicPlayer::setBgmVol(double vol)
{
    bgm->audioOutput()->setVolume(vol);

    // qDebug() << "set bgm volume to:" << vol;
}

void MusicPlayer::setSoundVol(double vol)
{
    btnsound->audioOutput()->setVolume(vol);
    clicksound->audioOutput()->setVolume(vol);

    // qDebug() << "set sound volume to:" << vol;
}

void MusicPlayer::changeBgm(QUrl url)
{
    //音乐淡入淡出，更换
    QSequentialAnimationGroup * anime = new QSequentialAnimationGroup;
    anime->addAnimation(Animation::MakeAnime(bgm->audioOutput(),"volume",500,Settings::musicVol,0));
    anime->addPause(500);
    anime->addAnimation(Animation::MakeAnime(bgm->audioOutput(),"volume",500,0,Settings::musicVol));


    QObject::connect(anime->animationAt(0),&QPropertyAnimation::finished,[this,url]{
        bgm->setSource(url);
    });
    QObject::connect(anime->animationAt(1),&QPropertyAnimation::finished,[this,url]{
        bgm->play();
    });

    anime->start(QAbstractAnimation::DeleteWhenStopped);

    // qDebug() << "set music to:" << url << " loops:" << bgm->loops() << " expect:" << QMediaPlayer::Infinite;
}

