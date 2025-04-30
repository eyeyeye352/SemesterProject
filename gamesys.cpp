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
    settingPage = new SettingPage;


    view = new QGraphicsView(this);
    view->setGeometry(0,0,Settings::screenWidth,Settings::screenHeight);
    view->setScene(startscene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    tempview = new QGraphicsView(this);
    tempview->setGeometry(0,0,Settings::screenWidth,Settings::screenHeight);
    tempview->setStyleSheet("background: transparent; border: none;");
    tempview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tempview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tempview->hide();



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

    for (int n = 0; n < startscene->levels.size(); ++n) {
        connect(startscene->levels[n],&LevelBlock::selected,this,&Gamesys::startGame);
    }

    //connections(levelScene)
    connect(gamescene->settingBtn,&GameBtn::clicked,this,&Gamesys::checkSetting);
    connect(gamescene->rankBtn,&GameBtn::clicked,this,&Gamesys::checkRank);
    connect(gamescene->backBtn,&GameBtn::clicked,[this]{
        Animation::changeScene(gamescene,startscene,view,1500);
        Animation::changeMusic(QUrl("qrc:/bgm/src/bgm/startSceneBGM.mp3"));
    });

    //connection(settingpage)
    connect(settingPage,&SettingPage::backHome,[this]{
        if(view->scene() == gamescene){
            Animation::changeScene(gamescene,startscene,view,1500);
        }
        tempview->hide();
    });
    connect(settingPage,&SettingPage::changeMusicVol,[this](double vol){
        Settings::musicVol = vol;
        MusicPlayer::getMPlayer()->setBgmVol(vol);
        qDebug() << "mvol" << MusicPlayer::getMPlayer()->bgm->audioOutput()->volume();
    });
    connect(settingPage,&SettingPage::changeSoundVol,[this](double vol){
        Settings::soundVol = vol;
        MusicPlayer::getMPlayer()->setSoundVol(vol);
        qDebug() << "svol" << MusicPlayer::getMPlayer()->btnsound->audioOutput()->volume()
                 << MusicPlayer::getMPlayer()->clicksound->audioOutput()->volume();
    });





}

Gamesys::~Gamesys()
{
    MusicPlayer::delPlayer();
}

void Gamesys::goLevelSelection(Mode mode)
{
    Animation::goLevelSelection(startscene);
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
    Animation::backModeSelection(startscene);
}



void Gamesys::checkSetting()
{
    //settingpage
    tempview->setScene(settingPage);
    tempview->show();
}


void Gamesys::checkRank()
{

}

void Gamesys::goCreateMode()
{

}

void Gamesys::startGame(QString levelInfo)
{
    Animation::changeScene(startscene,gamescene,view,2000);
    QTimer::singleShot(1000,[this,levelInfo]{
        gamescene->loadLevel(levelInfo);
    });
    Animation::changeMusic(QUrl("qrc:/bgm/src/bgm/ingameBgm.mp3"));
}












