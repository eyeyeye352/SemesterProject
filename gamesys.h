#ifndef GAMESYS_H
#define GAMESYS_H

#include "settings.h"
#include "musicplayer.h"
#include "scene.h"

class Gamesys : public QWidget
{
    Q_OBJECT

public:
    Gamesys(QWidget *parent = nullptr);
    ~Gamesys();

public slots:

    //进入关卡选择页面
    void goLevelSelection();

    //返回起始页面
    void backToStartScene();

    //功能纽
    void checkSetting();
    void checkRank();
    void goCreateMode();


private:
    StartScene* startscene;
    QGraphicsView* view;

    //timers
    QTimer * bgmoveTimer;

};
#endif // GAMESYS_H
