#ifndef SCENE_H
#define SCENE_H

#include "settings.h"
#include "musicplayer.h"
#include "gameobject.h"
#include "textblock.h"
#include "otheritems.h"
#include "gamesidebar.h"
#include "gamerecord.h"

class Scene : public QGraphicsScene
{

public:
    Scene(QObject *parent = nullptr);

protected:
    double maxZvalue();
};





class StartScene : public Scene{

    Q_OBJECT

public:
    StartScene(QObject *parent = nullptr);


    //循环播放的移动式场景
    QGraphicsPixmapItem *bg1, *bg2;

    //两个模式的按钮
    ClassicBtn *classicBtn;
    HexBtn *hexBtn;

    //title
    QGraphicsPixmapItem *title;

    //4个功能按钮
    FunctionBtn * settingBtn, *rankBtn, *createModeBtn ,*backBtn ,*saveBtn;

    //五个关卡
    QList<LevelSelectBlock*> levels;

public slots:
    void moveBG();

};




class LevelScene : public Scene{

    Q_OBJECT
    friend class Gamesys;

public:
    LevelScene(QObject* parent = nullptr);

    //设置、排行查看、存档按钮
    FunctionBtn *settingBtn, *rankBtn, *saveBtn ,*tipBtn ,*undoBtn ,*doBtn;

    //加入textblock
    void addTextBlock(TextBlock* t,int rows,int cols);

    //gamesys设置标题并排版
    void setTitle(QString);

    //gamesys设置游戏模式，（调整sideBar为对应的模式）
    void setGameMode(Mode);

    //设置步数显示
    void setStep(int);


signals:

    void clickBg();

private:

    QGraphicsTextItem * title;
    QGraphicsTextItem * stepText;

    MySideBar* sideBar;

};



//加载过度页面
class LoadScene : public Scene{

public:
    LoadScene(QObject* parent = nullptr);

private:
    //loading... 三个点轮流变换的效果
    QGraphicsTextItem* loadingText;
    int dotNum;
};




class SettingPage : public Scene
{
    Q_OBJECT
public:
    SettingPage(QObject *parent = nullptr);


signals:
    void backHome();
    void closeSetting();


private:
    ValSets *musicSli;
    ValSets *soundSli;
    FunctionBtn *backHomeBtn;
    FunctionBtn *closeSettingBtn;
    QGraphicsPixmapItem* bg;
};



class TipPage : public Scene
{
    Q_OBJECT

public:
    TipPage(QObject *parent = nullptr);

    void setAnswer(QString text,int cols);

signals:
    void closeTip();

private:
    QGraphicsPixmapItem* bg;
    QGraphicsTextItem* answer;
    FunctionBtn *closeTipBtn;
};




class RankPage : public Scene{};
class SavePage : public Scene{};



//游戏通关场景
class CompletePage : public Scene{

    Q_OBJECT
public:
    CompletePage(QObject *parent = nullptr);

    void setcontents(int step,int score);

signals:

    void backHome();


private:
    QGraphicsPixmapItem* bg;
    QGraphicsTextItem * stepText ,*score;
    FunctionBtn *homeBtn;


};



#endif // SCENE_H
