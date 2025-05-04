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

public:
    LevelScene(QObject* parent = nullptr);

    //设置、排行查看、存档按钮
    FunctionBtn *settingBtn, *rankBtn, *saveBtn ,*tipBtn ,*undoBtn ,*doBtn;

    //载入关卡内容并渲染
    void loadLevel(QString filepath);

    //重置关卡
    void reset();

    //核心玩法：选中blocks/交换blocks
    void selectBlocks(TextBlock*);
    void switchBlocks(TextBlock*);

    //细项
    void rowSelect(TextBlock*);
    void colSelect(TextBlock*);
    void crossSelect(TextBlock*);
    void cancelSelect();
    QList<TextBlock*> selectedBlocks;
    //十字变换专用容器
    QMap<QString,TextBlock*> crossTransMap;

    void rowSwitch(TextBlock*);
    void colSwitch(TextBlock*);
    void crossSwitch(TextBlock*);

    //设置游戏模式，（调整sideBar为对应的模式）
    void setGameMode(Mode);

    QString getLevelContent();
    int getCols();
    int getRows();



public slots:
    //更换方块选取/switch方式
    void changeTransType(TranslateIcons::Type);


private:

    //关卡内容
    QGraphicsTextItem * title;
    Mode mode;
    int rows,cols;

    //内容，同时作为解答
    QString contents;

    //方块们（小矩形或六边形）
    QList<TextBlock*> textBlocks;

    //根据当前是否有选中的block执行select或switch
    bool hasSelectBlocks;

    TranslateIcons::Type curTransType;
    MySideBar* sideBar;

    //gamerecords
    QList<GameRecord*> gameRecords;

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
class CompletePage : public Scene{};



#endif // SCENE_H
