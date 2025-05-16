#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "settings.h"

class MusicPlayer
{
    MusicPlayer();
    ~MusicPlayer();
    static MusicPlayer* instance;

public:

    //单例
    static MusicPlayer* getMPlayer(){
        if(instance == nullptr){
            instance = new MusicPlayer;
        }
        return instance;
    }

    static void delPlayer(){
        if(instance){
            delete instance;
        }
        instance = nullptr;
    }

    void startBGM();
    void startBtnSound();
    void clickSound();
    void playBlockSound(bool success);

    void setBgmVol(double vol);
    void setSoundVol(double vol);

    void changeBgm(QUrl);

    QMediaPlayer* bgm;
    QMediaPlayer* btnsound;
    QMediaPlayer* clicksound;
    QMediaPlayer* successBlockSound;
    QMediaPlayer* failBlockSound;


};

#endif // MUSICPLAYER_H
