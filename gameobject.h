#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "settings.h"




class GameObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:

    GameObject(QGraphicsItem *parent = nullptr);
    GameObject(QPixmap pixmap,QGraphicsItem *parent = nullptr);

};


class GameBtn : public GameObject
{
    Q_OBJECT

protected:

    GameBtn(QPixmap pixmap,QGraphicsItem *parent = nullptr);

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

public:
    FunctionBtn(QPixmap pixmap,QGraphicsItem *parent = nullptr);

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};



//level选择界面的level方块
class LevelBlock :  public GameBtn{

    Q_OBJECT

public:

    LevelBlock(int l,QGraphicsItem *parent = nullptr);

    //载入文件记录关卡信息
    void loadFile(QString);
    QString getInfo() const;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    //被选中的关卡,将关卡内容传出
    void selected(QString);

private:
    //哪一关
    int levelNum;
    //关卡详细信息
    QString info;

    Mode currentMode;

};



class BlackOverlay : public GameObject{

    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
public:
    BlackOverlay(QGraphicsItem *parent = nullptr);
};





#endif // GAMEOBJECT_H
