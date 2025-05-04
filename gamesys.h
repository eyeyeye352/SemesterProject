#ifndef GAMESYS_H
#define GAMESYS_H

#include "scene.h"
#include "settings.h"
#include "objpool.h"
#include "gamerecord.h"




class Gamesys : public QWidget
{
    Q_OBJECT

public:
    Gamesys(QWidget *parent = nullptr);
    ~Gamesys();

public slots:

    //进入关卡选择页面
    void goLevelSelection(Mode);

    //返回模式选择页面
    void backModeSelection();

    //打开，关闭设置
    void openSetting();
    void closeSetting();
    void backHome();

    //save
    void openSaveAndLoad();
    void closeSaveAndLoad();

    //tips
    void openTips();
    void closeTips();

    //rank
    void checkRank();

    //creation mode
    void goCreateMode();

    //选择关卡后进入游戏场景,参数为第几关
    void startGame(int);

    void completeGame();



private:
    StartScene* startscene;
    LevelScene* levelscene;
    SettingPage* settingPage;
    TipPage* tipPage;
    RankPage* rankPage;
    SavePage* savePage;
    CompletePage* complatePage;

    QGraphicsView* view , *tempview;

    //timers
    QTimer * bgmoveTimer;

    //current gameMode
    Mode currentMode;


};
















#endif // GAMESYS_H
