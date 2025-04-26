#ifndef SCENE_H
#define SCENE_H

#include "settings.h"
#include "musicplayer.h"
#include "levels.h"
class Scene : public QGraphicsScene
{

public:
    Scene(QObject *parent = nullptr);
};


class ClassicBtn : public QObject , public QGraphicsPixmapItem{

    Q_OBJECT
    // 声明 opacity 属性
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    // 声明 pos 属性
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    ClassicBtn(QGraphicsItem *parent = nullptr);

    //鼠标悬停特效
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    //点击效果
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked();
};

class HexBtn : public QObject , public QGraphicsPixmapItem{

    Q_OBJECT
    // 声明 opacity 属性
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    // 声明 pos 属性
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    HexBtn(QGraphicsItem *parent = nullptr);

    //鼠标悬停特效
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    //点击效果
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked();
};



class FunctionBtn : public QObject , public QGraphicsPixmapItem{

    Q_OBJECT
    // 声明 opacity 属性
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    // 声明 pos 属性
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    FunctionBtn(QPixmap pixmap,QGraphicsItem *parent = nullptr);

    //鼠标悬停特效
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    //点击效果
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked();
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

signals:



public slots:
    void moveBG();
};

#endif // SCENE_H
