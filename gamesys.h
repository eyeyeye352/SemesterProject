#ifndef GAMESYS_H
#define GAMESYS_H

#include "scene.h"
#include "settings.h"




class Gamesys : public QWidget
{
    Q_OBJECT

public:
    Gamesys(QWidget *parent = nullptr);
    ~Gamesys();

public slots:

    //进入关卡选择页面
    void goLevelSelection(Mode);

    //返回起始页面
    void backToStartScene();

    //功能纽
    void checkSetting();
    void checkRank();
    void goCreateMode();

    //选择关卡后进入游戏场景
    void startGame(QString);



private:
    StartScene* startscene;
    LevelScene* gamescene;
    QGraphicsView* view;

    //timers
    QTimer * bgmoveTimer;

    //current gameMode
    Mode currentMode;

};
#endif // GAMESYS_H
