#ifndef SETTINGS_H
#define SETTINGS_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QTimer>
#include <QObject>
#include <QWidget>
#include <QPointer>
#include <QDebug>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QGraphicsColorizeEffect>
#include <QGraphicsObject>
#include <QList>
#include <QFile>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QFontDatabase>

enum Mode{
    CLASSIC,
    HEX,
};

class Settings
{
    Settings();
public:

    static int screenWidth;
    static int screenHeight;

    static double backgroundMoveSpeed;
    static int backgroundUpdateInterval;

    //起始界面按钮位置
    static int classicBtnY;
    static int hexBtnY;

    //audio 音量
    static int musicVol;
    static int soundVol;

    //排版美观相关参数
    static double startBtnOriginScale;
    static double funcBtnOriginScale;
    static double funcBtnOriginOpacity;
    static double levelBlockOriginScale;
    static double levelBlockSpacing;
};

#endif // SETTINGS_H
