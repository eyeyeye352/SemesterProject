#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "settings.h"
#include "gamerecord.h"


class GameObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    GameObject(QGraphicsItem *parent = nullptr);
    GameObject(QPixmap pixmap,QGraphicsItem *parent = nullptr);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked();

};


class GameBtn : public GameObject
{
    Q_OBJECT

public:
    GameBtn(QPixmap pixmap,QGraphicsItem *parent = nullptr);

protected:

    //鼠标特效
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override{}
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override{}
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override{}
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override{}

    //中心缩放效果
    virtual void resizing(double scale);

signals:
    void clicked();
};





class ClassicBtn : public GameBtn
{
    Q_OBJECT
public:
    ClassicBtn(QGraphicsItem *parent = nullptr);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked(Mode);
};



class HexBtn : public GameBtn
{
    Q_OBJECT
public:
    HexBtn(QGraphicsItem *parent = nullptr);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clicked(Mode);
};



class FunctionBtn : public GameBtn
{
    Q_OBJECT
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)

public:
    FunctionBtn(QPixmap pixmap,QGraphicsItem *parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};



//level选择界面的方块
class LevelSelectBlock :  public GameBtn{

    Q_OBJECT

public:

    LevelSelectBlock(int l,QGraphicsItem *parent = nullptr);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    //被选中的关卡,将关卡内容传出
    void selected(int);

private:
    //哪一关
    int levelNum;

};


class TranslateIcons : public GameObject{

    Q_OBJECT

public:

    enum Type{
        NONE,
        ROWS,
        COLS,
        CROSS,
        HEXLINE,
        HEX
    };

    TranslateIcons(QPixmap pixmap,Type type,QGraphicsItem *parent = nullptr);
    void makeGold();
    void removeGold();

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
signals:
    void selected(Type);
private:
    Type type;
    QGraphicsRectItem* goldRect;
};




//标准的temppage中的按钮
class TempPageBtn : public GameBtn{

    Q_OBJECT

public:
    TempPageBtn(QGraphicsItem* parent);
    void setText(QString t);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:

    QGraphicsTextItem* text;
};



//savePage 的 slots按钮
//由slot储存信息
class SaveSlot : public QObject , public QGraphicsRectItem {

    Q_OBJECT
public:
    SaveSlot(QGraphicsItem* parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    //此处加载存档信息
    void init(QString filepath);

    //输出信息
    void info();


signals:
    void clicked();

private:

    QGraphicsTextItem* text;
    QString savetime;
    int levelNum,gameMode;
    QList<GameRecord> sysR,playerR;

};




#endif // GAMEOBJECT_H
