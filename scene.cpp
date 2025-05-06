#include "scene.h"
#include "objpool.h"
#include "myalgorithms.h"


Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}
{
    setSceneRect(QRectF(0,0,Settings::screenWidth,Settings::screenHeight));
}

double Scene::maxZvalue()
{
    double maxZ = 0;
    for (QGraphicsItem* item : items()) {
        if(item->zValue() > maxZ){
            maxZ = item->zValue();
        }
    }
    return maxZ;
}



StartScene::StartScene(QObject *parent)
    :Scene{parent}
{
    bg1 = new QGraphicsPixmapItem(QPixmap(":/background/src/background/start_background.png"));
    bg2 = new QGraphicsPixmapItem(QPixmap(":/background/src/background/start_background.png"));
    bg2->setX(Settings::screenWidth - 1);

    addItem(bg1);
    addItem(bg2);

    //btns
    classicBtn = new ClassicBtn;
    hexBtn = new HexBtn;

    classicBtn->setPos((Settings::screenWidth - classicBtn->sceneBoundingRect().width())/2,Settings::classicBtnY);
    hexBtn->setPos(Settings::screenWidth/2 - hexBtn->sceneBoundingRect().width()/2,Settings::hexBtnY);

    addItem(classicBtn);
    addItem(hexBtn);


    //title
    title = new QGraphicsPixmapItem(QPixmap(":/item/src/item/title.png"));
    title->setScale(Settings::startSceneTitleScale);
    //title x置中 y设高度50
    title->setPos((Settings::screenWidth - title->sceneBoundingRect().width())/2,
                  Settings::startSceneTitleY);
    addItem(title);

    //functional btns
    settingBtn = new FunctionBtn(QPixmap(":/item/src/item/settingIcon.png"));
    rankBtn = new FunctionBtn(QPixmap(":/item/src/item/rankIcon.png"));
    createModeBtn = new FunctionBtn(QPixmap(":/item/src/item/createModeIcon.png"));
    backBtn = new FunctionBtn(QPixmap(":/item/src/item/backIcon.png"));
    saveBtn = new FunctionBtn(QPixmap(":/item/src/item/saveIcon.png"));

    //功能按键列的位置排版
    settingBtn->setPos(Settings::functionBtnInterval,
                       Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    rankBtn->setPos(2*Settings::functionBtnInterval + settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    createModeBtn->setPos(3*Settings::functionBtnInterval + 2*settingBtn->sceneBoundingRect().width(),
                          Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    saveBtn->setPos(4*Settings::functionBtnInterval + 3*settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    backBtn->setPos(5*Settings::functionBtnInterval + 4*settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);




    //backBtn初始不可见。
    backBtn->setOpacity(0);

    addItem(settingBtn);
    addItem(rankBtn);
    addItem(createModeBtn);
    addItem(saveBtn);
    addItem(backBtn);


    //levelselectBlocks
    for (int n = 1; n <= 5; ++n) {
        LevelSelectBlock * l = new LevelSelectBlock(n);

        //排版
        l->moveBy(Settings::levelBlockSpacing*n + l->sceneBoundingRect().width()*(n-1),
                  (Settings::screenHeight - l->sceneBoundingRect().height())/2);

        l->setOpacity(0);

        //加进容器+场景
        levels.push_back(l);
        addItem(l);
    }
}


//背景交替循环移动。
void StartScene::moveBG()
{
    bg1->moveBy(-Settings::backgroundMoveSpeed,0);
    bg2->moveBy(-Settings::backgroundMoveSpeed,0);

    // - 1 小段重叠，优化边界闪烁问题
    if(bg1->sceneBoundingRect().right() < 0){
        bg1->setX(bg2->sceneBoundingRect().right() - 1);
    }
    if(bg2->sceneBoundingRect().right() < 0){
        bg2->setX(bg1->sceneBoundingRect().right() - 1);
    }

}





//加载场景
LoadScene::LoadScene(QObject *parent):
    Scene{parent},
    dotNum(0)
{
    addPixmap(QPixmap(":/background/src/background/loadScene1.png"));

    //底部半透明黑框
    QGraphicsRectItem * blackArea = new QGraphicsRectItem(0,Settings::screenHeight-Settings::screenHeight/5,
                                                         Settings::screenWidth,Settings::screenHeight/5);
    blackArea->setBrush(Qt::black);
    blackArea->setOpacity(0.5);
    addItem(blackArea);

    //loadingtext
    loadingText = new QGraphicsTextItem("Loading");

    //font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/AaYuanWeiTuSi-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,16);
    loadingText->setFont(font);
    loadingText->setDefaultTextColor(Qt::white);

    loadingText->setPos(Settings::loadingTextX,
                        blackArea->sceneBoundingRect().center().y() - loadingText->sceneBoundingRect().height()/2);

    addItem(loadingText);

    //loading...动画效果( 0~3个点 )
    QTimer* timer = new QTimer(this);
    timer->start(300);
    QObject::connect(timer,&QTimer::timeout,[this]{
        dotNum = (dotNum + 1) % 4;
        loadingText->setPlainText(QString("Loading%1").arg(QString(dotNum,'.')));
    });
}





//游戏内关卡场景
LevelScene::LevelScene(QObject *parent)
    :Scene{parent}
{
    GameObject * bg = new GameObject(QPixmap(":/background/src/background/ingame_background1.png"));
    connect(bg,&GameObject::clicked,[this]{ emit clickBg(); });
    addItem(bg);

    //functional btns
    settingBtn = new FunctionBtn(QPixmap(":/item/src/item/settingIcon.png"));
    rankBtn = new FunctionBtn(QPixmap(":/item/src/item/rankIcon.png"));
    saveBtn = new FunctionBtn(QPixmap(":/item/src/item/saveIcon.png"));
    tipBtn = new FunctionBtn(QPixmap(":/item/src/item/tipIcon.png"));
    undoBtn = new FunctionBtn(QPixmap(":/item/src/item/undoIcon.png"));
    doBtn = new FunctionBtn(QPixmap(":/item/src/item/doIcon.png"));


    settingBtn->setOpacity(0.5);
    rankBtn->setOpacity(0.5);
    saveBtn->setOpacity(0.5);
    tipBtn->setOpacity(0.5);
    undoBtn->setOpacity(0.5);
    doBtn->setOpacity(0.5);


    settingBtn->setPos(Settings::functionBtnInterval,
                       Settings::screenHeight - settingBtn->boundingRect().height() - 5);

    rankBtn->setPos(2*Settings::functionBtnInterval + settingBtn->boundingRect().width(),
                    Settings::screenHeight - settingBtn->boundingRect().height() - 5);
    saveBtn->setPos(3*Settings::functionBtnInterval + 2*settingBtn->boundingRect().width(),
                    Settings::screenHeight - settingBtn->boundingRect().height() - 5);
    tipBtn->setPos(4*Settings::functionBtnInterval + 3*settingBtn->boundingRect().width(),
                    Settings::screenHeight - settingBtn->boundingRect().height() - 5);
    undoBtn->setPos(5*Settings::functionBtnInterval + 4*settingBtn->boundingRect().width(),
                    Settings::screenHeight - settingBtn->boundingRect().height() - 5);
    doBtn->setPos(6*Settings::functionBtnInterval + 5*settingBtn->boundingRect().width(),
                  Settings::screenHeight - settingBtn->boundingRect().height() - 5);

    addItem(settingBtn);
    addItem(rankBtn);
    addItem(saveBtn);
    addItem(tipBtn);
    addItem(undoBtn);
    addItem(doBtn);


    //变换模式选择菜单栏
    sideBar = new MySideBar;
    sideBar->setPos(Settings::screenWidth - sideBar->btn->boundingRect().width(),0);
    addItem(sideBar);


    //title
    title = new QGraphicsTextItem(sideBar);

    //font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,18);
    title->setFont(font);
    title->setDefaultTextColor(Qt::white);

    //stepText
    stepText = new QGraphicsTextItem("已使用步数：0",bg);
    stepText->setFont(font);
    stepText->setDefaultTextColor(Qt::white);
    stepText->setPos(Settings::levelscene_stepTextPos);

}



void LevelScene::addTextBlock(TextBlock *t , int rows, int cols)
{
    //textblocks渲染
    double Width = cols * Settings::textBlockSize;
    double Height = rows * Settings::textBlockSize;
    double posx = (Settings::screenWidth - Width)/2 + t->getXY().x()*Settings::textBlockSize;
    double posy = (Settings::screenHeight - Height)/2 + t->getXY().y()*Settings::textBlockSize;

    t->setPos(posx,posy);
    addItem(t);
    sideBar->setZValue(t->zValue() + 1);
}


void LevelScene::setTitle(QString t)
{
    title->setPlainText(t.remove("title="));
    title->setPos((sideBar->boundingRect().width() - title->boundingRect().width())/2 + 10 , 600);
}



void LevelScene::setGameMode(Mode m)
{
    sideBar->setGameMode(m);
}

void LevelScene::setStep(int step)
{
    stepText->setPlainText(QString("已使用步数：%1").arg(QString::number(step)));
}






SettingPage::SettingPage(QObject *parent)
    :Scene{parent}
{

    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/settingPage.png"));
    addItem(bg);

    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);

    //Valsets
    musicSli = new ValSets(bg);
    soundSli = new ValSets(bg);
    musicSli->setPos(Settings::settingPage_MusicValSetPos);
    soundSli->setPos(Settings::settingPage_SoundValSetPos);

    //设置页面内部连接信号更改声音
    connect(musicSli,&ValSets::valueChanged,[this](double vol){
        Settings::musicVol = vol;
        MusicPlayer::getMPlayer()->setBgmVol(vol);
    });
    connect(soundSli,&ValSets::valueChanged,[this](double vol){
        Settings::soundVol = vol;
        MusicPlayer::getMPlayer()->setSoundVol(vol);
    });


    //btn
    backHomeBtn = new FunctionBtn(QPixmap(":/item/src/item/backHomeBtn.png"),bg);
    closeSettingBtn = new FunctionBtn(QPixmap(":/item/src/item/closeSettingBtn.png"),bg);


    backHomeBtn->setPos((bg->boundingRect().width() - backHomeBtn->boundingRect().width())/2,
                        bg->boundingRect().height() - backHomeBtn->boundingRect().height() - 25);

    closeSettingBtn->setPos({25,25});


    QObject::connect(backHomeBtn,&GameBtn::clicked,[this]{ emit backHome();});
    QObject::connect(closeSettingBtn,&GameBtn::clicked,[this]{ emit closeSetting(); });

}


TipPage::TipPage(QObject *parent):
    Scene(parent)
{
    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/tipPage.png"));
    addItem(bg);

    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);

    answer = new QGraphicsTextItem(bg);
    //font
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/tipPageTextFont.ttf",answer,Qt::white,16);

    //btn
    closeTipBtn = new FunctionBtn(QPixmap(":/item/src/item/closeSettingBtn.png"),bg);
    closeTipBtn->setPos(25,25);
    QObject::connect(closeTipBtn,&GameBtn::clicked,[this]{ emit closeTip(); });

}

void TipPage::setAnswer(QString text,int cols)
{
    QString ansText;
    for (int n = 0; n < text.size(); ++n) {
        if(n % cols == 0 && n != 0){
            ansText.append('\n');
        }
        ansText.append(text[n]);
    }

    qDebug() << "anstext:" << ansText;
    answer->setPlainText(ansText);
    answer->setPos((bg->boundingRect().width() - answer->boundingRect().width())/2,
                   100);

}





CompletePage::CompletePage(QObject *parent)
{
    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/completePage.png"));
    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);
    addItem(bg);

    stepText = new QGraphicsTextItem(bg);
    stepText->setPos(Settings::completeScene_stepTextPos);

    //font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,20);
    stepText->setFont(font);
    stepText->setDefaultTextColor(Qt::white);

    homeBtn = new FunctionBtn(QPixmap(":/item/src/item/backHomeBtn.png"),bg);
    homeBtn->setPos((bg->boundingRect().width() - homeBtn->boundingRect().width())/2,
                    bg->boundingRect().height() - homeBtn->boundingRect().height() - 25);
    connect(homeBtn,&GameBtn::clicked,[this]{ emit backHome();  });


}

void CompletePage::setcontents(int step)
{
    stepText->setPlainText(QString("Using step:        %1").arg(QString::number(step)));
}





SavePage::SavePage(QObject *parent):
    Scene(parent)
{
    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/savePage.png"));
    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);
    addItem(bg);

    s1 = new SaveSlot(bg);
    s2 = new SaveSlot(bg);
    s3 = new SaveSlot(bg);
    s4 = new SaveSlot(bg);

    s1->setRect(50,100,150,100);
    s2->setRect(250,100,150,100);
    s3->setRect(50,250,150,100);
    s4->setRect(250,250,150,100);

    connect(s1,&SaveSlot::clicked,[this]{ goLoading(1);});
    connect(s2,&SaveSlot::clicked,[this]{ goLoading(2);});
    connect(s3,&SaveSlot::clicked,[this]{ goLoading(3);});
    connect(s4,&SaveSlot::clicked,[this]{ goLoading(4);});

    back = new TempPageBtn(bg);
    save = new TempPageBtn(bg);
    load = new TempPageBtn(bg);
    back->setText("Back");
    save->setText("Save");
    load->setText("Load");

    back->setPos(bg->boundingRect().right() + 5, 50);
    save->setPos(bg->boundingRect().right() + 5, back->boundingRect().height() + 60);
    load->setPos(bg->boundingRect().right() + 5, 2*back->boundingRect().height() + 70);

}




