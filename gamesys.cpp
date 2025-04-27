#include "gamesys.h"
#include "animation.h"

Gamesys::Gamesys(QWidget *parent)
    : QWidget(parent)
{
    //窗口大小
    setFixedSize(Settings::screenWidth,Settings::screenHeight);

    //scene and view
    startscene = new StartScene(this);
    gamescene = new LevelScene(this);

    view = new QGraphicsView(this);
    view->setGeometry(0, 0, Settings::screenWidth, Settings::screenHeight);
    view->setScene(startscene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    //timers
    bgmoveTimer = new QTimer(this);
    bgmoveTimer->start(Settings::backgroundUpdateInterval);

    //timers actions
    connect(bgmoveTimer,&QTimer::timeout,startscene,&StartScene::moveBG);

    //bgm
    MusicPlayer::getMPlayer()->startBGM();

    //items connections(startScene)
    connect(startscene->classicBtn,&ClassicBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->hexBtn,&HexBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->settingBtn,&GameBtn::clicked,this,&Gamesys::checkSetting);
    connect(startscene->rankBtn,&GameBtn::clicked,this,&Gamesys::checkRank);
    connect(startscene->createModeBtn,&GameBtn::clicked,this,&Gamesys::goCreateMode);
    connect(startscene->backBtn,&GameBtn::clicked,this,&Gamesys::backToStartScene);

    //items connections(levelScene)
    connect(gamescene->settingBtn,&GameBtn::clicked,this,&Gamesys::checkSetting);
    connect(gamescene->rankBtn,&GameBtn::clicked,this,&Gamesys::checkRank);
    connect(gamescene->backBtn,&GameBtn::clicked,[this]{
        Animation::backToHost(gamescene,startscene,view);
        Animation::changeMusic(QUrl("qrc:/bgm/src/bgm/startSceneBGM.mp3"));
    });

    for (int n = 0; n < startscene->levels.size(); ++n) {
        connect(startscene->levels[n],&LevelBlock::selected,this,&Gamesys::startGame);
    }

}

Gamesys::~Gamesys()
{
    MusicPlayer::delPlayer();
}

void Gamesys::goLevelSelection(Mode mode)
{
    Animation::switchToLevelPage(startscene);
    currentMode = mode;
    qDebug() << "mode:" << mode;

    //load levels
    for (int n = 0; n < startscene->levels.size(); ++n) {

        QString filepath;

        switch(currentMode){
        case Mode::CLASSIC:
            filepath = QString(":/Levels/src/levelsDoc/ClassicL%1.txt").arg(n+1);
            break;
        case Mode::HEX:
            filepath = QString(":/Levels/src/levelsDoc/HexL%1.txt").arg(n+1);
        }

        startscene->levels[n]->loadFile(filepath);
    };
}


void Gamesys::backToStartScene()
{
    Animation::backToHost(startscene);
}



void Gamesys::checkSetting()
{

}


void Gamesys::checkRank()
{

}

void Gamesys::goCreateMode()
{

}

void Gamesys::startGame(QString levelInfo)
{
    gamescene->loadLevel(levelInfo);
    Animation::transToGame(startscene,gamescene,view);
    Animation::changeMusic(QUrl("qrc:/bgm/src/bgm/ingameBgm.mp3"));
}












