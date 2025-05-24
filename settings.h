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
#include <QSlider>
#include <QGraphicsWidget>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>
#include <QTextCharFormat>
#include <QTextFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextBlockFormat>
#include <QTextFrame>
#include <QRandomGenerator>
#include <QStack>
#include <QMessageBox>
#include <QLabel>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QTextEdit>

class SideBar;

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
    static QString gameIconPath;

    static double backgroundMoveSpeed;
    static int backgroundUpdateInterval;

    //起始界面按钮位置
    static int classicBtnY;
    static int hexBtnY;

    //audio 音量
    static double musicVol;
    static double soundVol;

    //排版美观相关参数
    static double startBtnOriginScale;
    static double funcBtnOriginScale;
    static double funcBtnOriginOpacity;
    static double levelBlockOriginScale;
    static double levelBlockSpacing;

    static double startSceneTitleScale;
    static int startSceneTitleY;
    static int levelSceneTiTleY;

    static int loadingTextX;

    static int functionBtnInterval;

    //关卡文字方块大小
    static double textBlockSize;
    static double hexBlockSize;

    //设置页面大小
    static QPointF settingPage_MusicValSetPos;
    static QPointF settingPage_SoundValSetPos;

    //游戏内--变换模式选择菜单栏
    static int sideBarWidth;
    static int translateIconInterval;

    //levelscene stepText位置
    static QPointF levelscene_stepTextPos;

    static QPointF completeScene_stepTextPos;

    //bgmList
    static QMap<QString,QUrl> bgmList;
};

#endif // SETTINGS_H
