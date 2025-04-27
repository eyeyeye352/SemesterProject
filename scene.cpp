#include "scene.h"
#include "animation.h"


Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}
{}

StartScene::StartScene(QObject *parent)
    :Scene{parent}
{

    setSceneRect(QRectF(0,0,Settings::screenWidth,Settings::screenHeight));
    bg1 = new QGraphicsPixmapItem(QPixmap(":/background/src/background/start_background.png"));
    bg2 = new QGraphicsPixmapItem(QPixmap(":/background/src/background/start_background.png"));
    bg2->setX(Settings::screenWidth - 1);

    addItem(bg1);
    addItem(bg2);

    //btns
    classicBtn = new ClassicBtn();
    hexBtn = new HexBtn();

    classicBtn->setScale(2);
    hexBtn->setScale(2);

    classicBtn->setPos((Settings::screenWidth - classicBtn->sceneBoundingRect().width())/2,Settings::classicBtnY);
    hexBtn->setPos(Settings::screenWidth/2 - hexBtn->sceneBoundingRect().width()/2,Settings::hexBtnY);

    addItem(classicBtn);
    addItem(hexBtn);


    //title
    title = new QGraphicsPixmapItem(QPixmap(":/item/src/item/title.png"));
    title->setScale(1.3);
    title->setPos((Settings::screenWidth - title->sceneBoundingRect().width())/2,50);
    addItem(title);

    //functional btns
    settingBtn = new FunctionBtn(QPixmap(":/item/src/item/settingIcon.png"));
    rankBtn = new FunctionBtn(QPixmap(":/item/src/item/rankIcon.png"));
    createModeBtn = new FunctionBtn(QPixmap(":/item/src/item/createModeIcon.png"));
    backBtn = new FunctionBtn(QPixmap(":/item/src/item/backIcon.png"));

    settingBtn->setPos(5,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    rankBtn->setPos(10 + settingBtn->sceneBoundingRect().width() , Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    createModeBtn->setPos(15 + 2*settingBtn->sceneBoundingRect().width() ,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    backBtn->setPos(20 + 3*settingBtn->sceneBoundingRect().width() ,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    backBtn->setOpacity(0);

    addItem(settingBtn);
    addItem(rankBtn);
    addItem(createModeBtn);
    addItem(backBtn);


    //levelBlocks
    for (int n = 1; n <= 5; ++n) {
        LevelBlock * l = new LevelBlock(n);
        //排版
        l->moveBy(Settings::levelBlockSpacing*n + l->sceneBoundingRect().width()*(n-1),
                  (Settings::screenHeight - l->sceneBoundingRect().height())/2);

        levels.push_back(l);
        l->setOpacity(0);
        addItem(l);
    }



}

StartScene::~StartScene(){}


void StartScene::moveBG()
{

    bg1->moveBy(-Settings::backgroundMoveSpeed,0);
    bg2->moveBy(-Settings::backgroundMoveSpeed,0);

    // - 1 优化边界闪烁问题
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

    //半透明黑框
    QGraphicsRectItem * blackArea = new QGraphicsRectItem(0,Settings::screenHeight-Settings::screenHeight/5,Settings::screenWidth,Settings::screenHeight/5);
    blackArea->setBrush(Qt::black);
    blackArea->setOpacity(0.5);
    addItem(blackArea);

    //loadingtext
    loadingText = new QGraphicsTextItem("Loading.");

    //(font)
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/AaYuanWeiTuSi-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,16);
    loadingText->setFont(font);
    loadingText->setDefaultTextColor(Qt::white);

    loadingText->moveBy(10,blackArea->sceneBoundingRect().center().y() - loadingText->sceneBoundingRect().height()/2);
    addItem(loadingText);

    //text: loading...anime( 0~3个点 )
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
    setSceneRect(QRectF(0,0,Settings::screenWidth,Settings::screenHeight));
    QGraphicsPixmapItem * bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/ingame_background1.png"));
    addItem(bg);

    //functional btns
    settingBtn = new FunctionBtn(QPixmap(":/item/src/item/settingIcon.png"));
    rankBtn = new FunctionBtn(QPixmap(":/item/src/item/rankIcon.png"));
    backBtn = new FunctionBtn(QPixmap(":/item/src/item/backIcon.png"));

    settingBtn->setPos(5,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    rankBtn->setPos(10 + settingBtn->sceneBoundingRect().width() , Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
    backBtn->setPos(15 + 2*settingBtn->sceneBoundingRect().width() ,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    settingBtn->setOpacity(Settings::funcBtnOriginOpacity*0.5);
    rankBtn->setOpacity(Settings::funcBtnOriginOpacity*0.5);
    backBtn->setOpacity(Settings::funcBtnOriginOpacity*0.5);

    addItem(settingBtn);
    addItem(rankBtn);
    addItem(backBtn);

    //title
    title = new QGraphicsTextItem();
    //style
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,25);
    title->setFont(font);
    title->setDefaultTextColor(Qt::white);
    title->setPos((Settings::screenWidth - title->sceneBoundingRect().width())/2,Settings::screenHeight/10);
    addItem(title);

}

void LevelScene::loadLevel(QString levelInfo)
{
    info = levelInfo;
    QStringList strList = info.split("\r\n");
    QString titleName;

    //字符串处理
    for (int line = 0; line < strList.size(); ++line) {
        if(strList[line].startsWith("title=")){
            titleName = strList[line].remove("title=");
        }
        else if(strList[line].startsWith("difficulty=")){
            QStringList nums = strList[line].remove("difficulty=").split(',');
            rows = nums[0].toInt();
            cols = nums[1].toInt();
        }
        else if(strList[line].startsWith("content=")){
            for (int y = line+1; y < line+1+cols; ++y) {
                contents.append(strList[y]);
            }
            //读取结束
            break;
        }
    }

    //test(succeed)
    // qDebug() << "title: " << title;
    // qDebug() << "r/c: " << rows << "/" << cols;
    // qDebug() << "content" << contents;

    title->setPlainText(titleName);


}



