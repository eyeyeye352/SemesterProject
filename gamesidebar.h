#ifndef GAMESIDEBAR_H
#define GAMESIDEBAR_H

#include "settings.h"
#include "gameobject.h"


//侧边栏
class SideBar : public GameObject{

    Q_OBJECT

public:
    SideBar(QGraphicsItem *parent = nullptr);

    FunctionBtn* btn;
    virtual QRectF boundingRect() const override;


public slots:
    void openOrCloseSideBar();

protected:

    bool opened;
    QGraphicsRectItem* bg;
};






//根据模式不同而存放不同translateIcons的MySideBar
class MySideBar : public SideBar{

    Q_OBJECT

public:
    MySideBar(QGraphicsItem *parent = nullptr);

    void setGameMode(Mode mode);

signals:
    void SelectTransType(TranslateIcons::Type);

public slots:
    void makeIconGold(TranslateIcons*);

private:
    //变换模式选择icons(classicMode)
    QList<TranslateIcons*> classicIcons;
    QList<TranslateIcons*> hexIcons;

    Mode gameMode;

};

#endif // GAMESIDEBAR_H
