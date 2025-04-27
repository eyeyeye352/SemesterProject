#ifndef SCENE_H
#define SCENE_H

#include "settings.h"
#include "musicplayer.h"
#include "gameobject.h"
#include "textblock.h"

class Scene : public QGraphicsScene
{

public:
    Scene(QObject *parent = nullptr);
};





class StartScene : public Scene{

    Q_OBJECT

public:
    StartScene(QObject *parent = nullptr);
    ~StartScene();


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
    QList<LevelBlock*> levels;

    Mode currentMode;

public slots:
    void moveBG();
};




class LevelScene : public Scene{
public:
    LevelScene(QObject* parent = nullptr);

    //三个功能按钮
    FunctionBtn *settingBtn, *rankBtn, *backBtn;

    //关卡元素们
    QList<TextBlock*> textBlocks;

    //载入关卡内容
    void loadLevel(QString levelInfo);

    //关卡内容
    QString info;
    QGraphicsTextItem * title;
    QString contents;
    int cols,rows;
};



class LoadScene : public Scene{
public:
    LoadScene(QObject* parent = nullptr);


private:
    QGraphicsTextItem* loadingText;
    int dotNum;
};

#endif // SCENE_H
