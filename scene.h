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
    FunctionBtn * settingBtn, *createModeBtn ,*backBtn ,*saveBtn;

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
    void addTextBlock(RectTextBlock* t,int rows,int cols);

    //加入hexblock
    void addTextBlock(HexTextBlock* h);
    void addCenterHexBlock(HexTextBlock* h,int radius);
    void processNeightbors(HexTextBlock* center,int radius);

    //自定义关卡不支持存档与排行
    void enterFromCreateScene();
    void recover();


    //gamesys设置标题并排版
    void setTitle(QString);

    //gamesys设置游戏模式，（调整sideBar为对应的模式）
    void setGameMode(int);

    //设置步数显示
    void setStep(int);

signals:

    void clickBg();

private:

    QGraphicsTextItem * title;
    QGraphicsTextItem * stepText;

    MySideBar* sideBar;

    //辅助设置hexblock的R用
    int curR;

};



//加载过度页面
class LoadScene : public Scene{

public:
    LoadScene(QObject* parent = nullptr);
    ~LoadScene();

private:
    //loading... 三个点轮流变换的效果
    QPointer<QGraphicsTextItem> loadingText;
    int dotNum;
};


class CreateScene : public Scene{

public:
    CreateScene(QObject* parent = nullptr);

    //循环播放的移动式场景
    GameObject *bg1;

    //title
    QGraphicsPixmapItem *title;

    //4个功能按钮
    FunctionBtn * settingBtn, *backBtn;

    //关卡
    FunctionBtn* levelblock,*buildblock;

};



#endif // SCENE_H
