#include "gamesys.h"
#include "animation.h"

Gamesys::Gamesys(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(Settings::screenWidth,Settings::screenHeight);

    startscene = new StartScene(this);
    view = new QGraphicsView(this);
    view->setGeometry(0, 0, Settings::screenWidth, Settings::screenHeight);
    view->setScene(startscene);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate); // 确保整个视口更新

    //timers
    bgmoveTimer = new QTimer(this);
    bgmoveTimer->start(Settings::backgroundUpdateInterval);
    connect(bgmoveTimer,&QTimer::timeout,startscene,&StartScene::moveBG);

    //bgm
    MusicPlayer::getMPlayer()->startBGM();

    //connections
    connect(startscene->classicBtn,&ClassicBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->hexBtn,&HexBtn::clicked,this,&Gamesys::goLevelSelection);

    connect(startscene->settingBtn,&FunctionBtn::clicked,this,&Gamesys::checkSetting);
    connect(startscene->rankBtn,&FunctionBtn::clicked,this,&Gamesys::checkRank);
    connect(startscene->createModeBtn,&FunctionBtn::clicked,this,&Gamesys::goCreateMode);
    connect(startscene->backBtn,&FunctionBtn::clicked,this,&Gamesys::backToStartScene);

}

Gamesys::~Gamesys()
{
    MusicPlayer::delPlayer();
}

void Gamesys::goLevelSelection()
{
    Animation::switchToLevelPage(startscene);

}

void Gamesys::backToStartScene()
{
    Animation::backToHost(startscene);
}

//还有一个backtohost

void Gamesys::checkSetting()
{

}

void Gamesys::checkRank()
{

}

void Gamesys::goCreateMode()
{

}












