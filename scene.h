#ifndef SCENE_H
#define SCENE_H

#include "settings.h"
#include "musicplayer.h"
#include "gameobject.h"
#include "textblock.h"
#include "otheritems.h"

class Scene : public QGraphicsScene
{

public:
    Scene(QObject *parent = nullptr);
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
    FunctionBtn * settingBtn, *rankBtn, *createModeBtn ,*backBtn;

    //五个关卡
    QList<LevelSelectBlock*> levels;

public slots:
    void moveBG();

};




class LevelScene : public Scene{
public:
    LevelScene(QObject* parent = nullptr);

    //设置、排行查看、存档按钮
    FunctionBtn *settingBtn, *rankBtn, *saveBtn;

    //载入关卡内容并渲染
    void loadLevel(QString filepath);

    //优化内存释放控件
    void release();

private:

    //关卡内容
    QGraphicsTextItem * title;
    Mode mode;
    int rows,cols;
    QString contents;


    //方块们（小矩形或六边形）
    QList<TextBlock*> textBlocks;

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



#endif // SCENE_H
