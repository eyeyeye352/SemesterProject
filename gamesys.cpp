#include "gamesys.h"
#include "animation.h"


Gamesys::Gamesys(QWidget *parent)
    : QWidget(parent)
{
    //窗口设置
    setFixedSize(Settings::screenWidth,Settings::screenHeight);
    setWindowIcon(QIcon(Settings::gameIconPath));

    //scene and view
    startscene = new StartScene(this);
    levelscene = new LevelScene(this);

    view = new QGraphicsView(this);
    view->setGeometry(0,0,Settings::screenWidth,Settings::screenHeight);
    view->setScene(startscene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //tempview用于显示设置、rank等子页面
    tempview = new QGraphicsView(this);
    tempview->setGeometry(0,0,Settings::screenWidth,Settings::screenHeight);
    tempview->setStyleSheet("background: transparent; border: none;");
    tempview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tempview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tempview->hide();

    settingPage = new SettingPage(this);



    //timers
    bgmoveTimer = new QTimer(this);
    bgmoveTimer->start(Settings::backgroundUpdateInterval);
    connect(bgmoveTimer,&QTimer::timeout,startscene,&StartScene::moveBG);

    //bgm
    MusicPlayer::getMPlayer()->startBGM();

    //startScene connections
    connect(startscene->classicBtn,&ClassicBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->hexBtn,&HexBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->settingBtn,&GameBtn::clicked,this,&Gamesys::openSetting);
    connect(startscene->rankBtn,&GameBtn::clicked,this,&Gamesys::checkRank);
    connect(startscene->createModeBtn,&GameBtn::clicked,this,&Gamesys::goCreateMode);
    connect(startscene->backBtn,&GameBtn::clicked,this,&Gamesys::backModeSelection);

    for (int n = 0; n < startscene->levels.size(); ++n) {
        connect(startscene->levels[n],&LevelSelectBlock::selected,this,&Gamesys::startGame);
    }

    //connections(levelScene)
    connect(levelscene->settingBtn,&GameBtn::clicked,this,&Gamesys::openSetting);
    connect(levelscene->rankBtn,&GameBtn::clicked,this,&Gamesys::checkRank);
    connect(levelscene->saveBtn,&GameBtn::clicked,this,&Gamesys::openSaveAndLoad);
    connect(levelscene->tipBtn,&GameBtn::clicked,this,&Gamesys::openTips);

    //connection(settingpage)
    connect(settingPage,&SettingPage::backHome,this,&Gamesys::backHome);
    connect(settingPage,&SettingPage::closeSetting,this,&Gamesys::closeSetting);



}

Gamesys::~Gamesys()
{

    MusicPlayer::delPlayer();
    objPool::delinstance();
}

void Gamesys::goLevelSelection(Mode mode)
{
    Animation::goLevelSelection(startscene);
    currentMode = mode;

    qDebug() << "choose mode:" << currentMode;
}


void Gamesys::backModeSelection()
{
    Animation::backModeSelection(startscene);
}



void Gamesys::openSetting()
{
    tempview->setScene(settingPage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);
}

void Gamesys::closeSetting()
{
    QPropertyAnimation* anime = Animation::TempPageout(view,tempview);
    anime->start(QAbstractAnimation::DeleteWhenStopped);

    connect(anime,&QPropertyAnimation::finished,[this]{
        tempview->setScene(nullptr);
    });

}

void Gamesys::backHome()
{

    if(view->scene() == startscene){
        closeSetting();
    }

    //关闭tempview动画结束后，返回首页
    else if(view->scene() == levelscene){

        QPropertyAnimation* anime = Animation::TempPageout(view,tempview);

        connect(anime,&QPropertyAnimation::finished,[this]{
            Animation::changeScene(levelscene,startscene,view,1500)->start(QAbstractAnimation::DeleteWhenStopped);
            MusicPlayer::getMPlayer()->changeBgm(QUrl("qrc:/bgm/src/bgm/startSceneBGM.mp3"));
            tempview->setScene(nullptr);
            levelscene->reset();
        });

        anime->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void Gamesys::openSaveAndLoad()
{

}

void Gamesys::closeSaveAndLoad()
{

}

void Gamesys::openTips()
{

}

void Gamesys::closeTips()
{

}


void Gamesys::checkRank()
{

}

void Gamesys::goCreateMode()
{

}

void Gamesys::startGame(int levelNum)
{

    //根据模式选择关卡文件
    QString filepath;
    switch(currentMode){
    case CLASSIC:
        filepath = QString(":/Levels/src/levelsDoc/ClassicL%1.txt").arg(levelNum);
        break;
    case HEX:
        filepath = QString(":/Levels/src/levelsDoc/HexL%1.txt").arg(levelNum);
        break;
    }


    //切换音乐和scene
    MusicPlayer::getMPlayer()->changeBgm(QUrl("qrc:/bgm/src/bgm/ingameBgm.mp3"));
    QSequentialAnimationGroup* anime = Animation::changeScene(startscene,levelscene,view,3000);
    connect(anime->animationAt(1),&QPropertyAnimation::finished,[this,filepath]{
        //动画暂停时加载文件
        levelscene->loadLevel(filepath);
    });
    anime->start(QAbstractAnimation::DeleteWhenStopped);

}













