#ifndef GAMESYS_H
#define GAMESYS_H

#include "scene.h"
#include "settings.h"
#include "objpool.h"




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

    //打开，关闭设置
    void openSetting();
    void closeSetting();

    void backHome();


    void checkRank();
    void goCreateMode();

    //选择关卡后进入游戏场景,参数为第几关
    void startGame(int);



private:
    StartScene* startscene;
    LevelScene* levelscene;
    SettingPage* settingPage;
    QGraphicsView* view , *tempview;

    //timers
    QTimer * bgmoveTimer;

    //current gameMode
    Mode currentMode;

};
#endif // GAMESYS_H
