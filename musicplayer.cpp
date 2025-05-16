#include "musicplayer.h"
#include "animation.h"




MusicPlayer* MusicPlayer::instance = nullptr;

MusicPlayer::MusicPlayer() {

    bgm = new QMediaPlayer;
    btnsound = new QMediaPlayer;
    clicksound = new QMediaPlayer;
    successBlockSound = new QMediaPlayer;
    failBlockSound = new QMediaPlayer;

    QAudioOutput *o1 = new QAudioOutput;
    QAudioOutput *o2 = new QAudioOutput;
    QAudioOutput *o3 = new QAudioOutput;
    QAudioOutput *o4 = new QAudioOutput;
    QAudioOutput *o5 = new QAudioOutput;

    bgm->setAudioOutput(o1);
    btnsound->setAudioOutput(o2);
    clicksound->setAudioOutput(o3);
    successBlockSound->setAudioOutput(o4);
    failBlockSound->setAudioOutput(o5);

    bgm->setSource(Settings::bgmList["pastel_subminal"]);
    btnsound->setSource(QUrl("qrc:/audio/src/sound/btnHoverSound.mp3"));
    clicksound->setSource(QUrl("qrc:/audio/src/sound/btnclick.mp3"));
    successBlockSound->setSource(QUrl("qrc:/audio/src/sound/finishSwitchBlock.mp3"));
    failBlockSound->setSource(QUrl("qrc:/audio/src/sound/switchblockFail.mp3"));

    o1->setVolume(Settings::musicVol);
    o2->setVolume(Settings::soundVol);
    o3->setVolume(Settings::soundVol);
    o4->setVolume(Settings::soundVol);
    o5->setVolume(Settings::soundVol);



    /* debug
     *
     * qDebug() << "musicvol: " << o1->volume() << "expect: " << Settings::musicVol;
     * qDebug() << "soundvol: " << o2->volume() << ',' << o3->volume() << "expect: " << Settings::soundVol;
    */
}

MusicPlayer::~MusicPlayer()
{}

void MusicPlayer::startBGM()
{
    bgm->setLoops(QMediaPlayer::Infinite);
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

void MusicPlayer::playBlockSound(bool success)
{
    if(success) {
        successBlockSound->setPosition(0);
        successBlockSound->play();
    }else{
        failBlockSound->setPosition(0);
        failBlockSound->play();
    }
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
    successBlockSound->audioOutput()->setVolume(vol);
    failBlockSound->audioOutput()->setVolume(vol);

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
        bgm->setLoops(QMediaPlayer::Infinite);
        bgm->play();
    });

    anime->start(QAbstractAnimation::DeleteWhenStopped);

    // qDebug() << "set music to:" << url << " loops:" << bgm->loops() << " expect:" << QMediaPlayer::Infinite;
}

