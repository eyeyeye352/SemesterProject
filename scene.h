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
    void setGameMode(int);

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



//设置、排行、提示等页面的基类
class TempPage : public Scene{

    Q_OBJECT
public:
    TempPage(QObject *parent);

    TempPageBtn* addBtn(QString text,QGraphicsItem* btnParent = nullptr);

signals:
    //关闭页面信号，一般在back按钮被点击时发出，请求gamesys关闭该场景。
    void closePage();

protected:
    QList<TempPageBtn*> btns;
};




class SettingPage : public TempPage
{
    Q_OBJECT
public:
    SettingPage(QObject *parent = nullptr);

signals:
    void goHome();

private:

    QGraphicsPixmapItem* bg;
    ValSets *musicSli;
    ValSets *soundSli;
    QPointer<TempPageBtn> homeBtn;
    QPointer<TempPageBtn> backBtn;
};



class TipPage : public TempPage
{
    Q_OBJECT

public:
    TipPage(QObject *parent = nullptr);

    void setAnswer(QString text,int cols);

private:

    QGraphicsPixmapItem* bg;
    QGraphicsTextItem* answer;
    QPointer<TempPageBtn> backBtn;
};




class RankPage : public TempPage{

    Q_OBJECT
public:

    RankPage(QObject* parent);

private:
    QGraphicsPixmapItem* bg;
};





class SavePage : public TempPage{

    Q_OBJECT

public:
    SavePage(QObject *parent = nullptr);

    void backToNoneState();
    void addBlack(int newState);
    void hideSaveBtn();
    void showSaveBtn();

    QList<SaveSlot*>& getSlots();

    //当前页面的选择状态，包括：
    //1. None，此时点击back会发出closeTempPage信号，点击save和load进入对应状态
    //2. load, 此时back会返回NONE状态，save和load禁用（背景变暗突出back和slots）
    //3. save，同理
    enum State{
        NONE,
        LOAD,
        SAVE
    };

signals:
    void slotSelected(int slotNum,int state);
    void saveAt(int slotNum);
    void loadAt(int slotNum);

private:


    QGraphicsPixmapItem* bg;

    //四个槽位
    QList<SaveSlot*> SLslots;
    TempPageBtn *back,*save,*load;

    //黑幕效果，state为LOAD或SAVE时凸显slot及back用
    QGraphicsRectItem* blackLay;
    int state;
};



//游戏通关场景
class CompletePage : public TempPage{

    Q_OBJECT
public:
    CompletePage(QObject *parent = nullptr);

    void showUseStep(int step);

signals:
    void goHome();


private:

    QGraphicsPixmapItem* bg;
    QGraphicsTextItem * stepText;
    QPointer<TempPageBtn> homeBtn;

};



#endif // SCENE_H
