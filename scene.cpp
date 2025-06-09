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
    createModeBtn = new FunctionBtn(QPixmap(":/item/src/item/createModeIcon.png"));
    backBtn = new FunctionBtn(QPixmap(":/item/src/item/backIcon.png"));
    saveBtn = new FunctionBtn(QPixmap(":/item/src/item/saveIcon.png"));

    //功能按键列的位置排版
    settingBtn->setPos(Settings::functionBtnInterval,
                       Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    createModeBtn->setPos(2*Settings::functionBtnInterval + settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    saveBtn->setPos(3*Settings::functionBtnInterval + 2*settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    backBtn->setPos(4*Settings::functionBtnInterval + 3*settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);




    //backBtn初始不可见。
    backBtn->setOpacity(0);

    addItem(settingBtn);
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

LoadScene::~LoadScene()
{
    clear();
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
    stepText = new QGraphicsTextItem(bg);
    stepText->setPos(5,5);

    //font
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",title,Qt::white,18);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",stepText,Qt::white,18);

}



void LevelScene::addTextBlock(RectTextBlock *t , int rows, int cols)
{

    //textblocks排版
    double Width = cols * Settings::textBlockSize;
    double Height = rows * Settings::textBlockSize;
    double posx = (Settings::screenWidth - Width)/2 + t->getXY().x()*Settings::textBlockSize;
    double posy = (Settings::screenHeight - Height)/2 + t->getXY().y()*Settings::textBlockSize;

    t->setPos(posx,posy);
    addItem(t);
    sideBar->setZValue(t->zValue() + 1);
}

void LevelScene::addTextBlock(HexTextBlock *h)
{
    addItem(h);
    sideBar->setZValue(h->zValue() + 1);
}

void LevelScene::addCenterHexBlock(HexTextBlock *h,int size)
{
    addTextBlock(h);
    h->setPos(Settings::screenWidth/2,Settings::screenHeight/2);
    h->setIsPainted(true);

    processNeightbors(h,size);
}

void LevelScene::processNeightbors(HexTextBlock *center,int blockSize)
{
    QList<HexTextBlock*>& neighbors = center->getNeightBors();

    for (int n = 0; n < neighbors.size(); ++n) {

        HexTextBlock* neib = neighbors[n];

        if(neib->getIsPainted()){
            continue;
        }

        //两个六边形中心相距 = sqrt(3)*size
        double distance = sqrt(3)*blockSize;
        //邻居位置相对自身位置
        QPointF newPos(center->pos());

        //注意：Qt y轴向下为正
        //右
        if(neib->getXY() - center->getXY() == QPoint(1,0)){
            newPos += QPointF(distance,0);
        }
        //右上
        else if(neib->getXY() - center->getXY() == QPoint(1,1)){
            newPos += QPointF(distance*cos(M_PI/3),-distance*sin(M_PI/3));
        }
        //左上
        else if(neib->getXY() - center->getXY() == QPoint(0,1)){
            newPos += QPointF(distance*cos(2*M_PI/3),-distance*sin(2*M_PI/3));
        }
        //左
        else if(neib->getXY() - center->getXY() == QPoint(-1,0)){
            newPos += QPointF(-distance,0);
        }
        //左下
        else if(neib->getXY() - center->getXY() == QPoint(-1,-1)){
            newPos += QPointF(distance*cos(4*M_PI/3),-distance*sin(4*M_PI/3));
        }
        //右下
        else if(neib->getXY() - center->getXY() == QPoint(0,-1)){
            newPos += QPointF(distance*cos(5*M_PI/3),-distance*sin(5*M_PI/3));
        }

        //设置位置
        neib->setPos(newPos);
        neib->setIsPainted(true);
        addItem(neib);
        processNeightbors(neib,blockSize);
    }
}

void LevelScene::enterFromCreateScene()
{
    saveBtn->setOpacity(0);
    rankBtn->setOpacity(0);
}

void LevelScene::recover()
{
    saveBtn->setOpacity(1);
    rankBtn->setOpacity(1);
}


void LevelScene::setTitle(QString t)
{
    title->setPlainText(t.remove("title="));
    title->setPos((sideBar->boundingRect().width() - title->boundingRect().width())/2 + 10 , 600);
}



void LevelScene::setGameMode(int m)
{
    sideBar->setGameMode(m);
}

void LevelScene::setStep(int step)
{
    stepText->setPlainText(QString("已使用步数：%1").arg(QString::number(step)));
}




CreateScene::CreateScene(QObject *parent)
    :Scene{parent}
{
    bg1 = new GameObject(QPixmap(":/background/src/background/createscene.png"));
    bg1->moveBy((Settings::screenWidth - bg1->boundingRect().width())/2,
                (Settings::screenHeight - bg1->boundingRect().height())/2);
    addItem(bg1);


    //title
    title = new QGraphicsPixmapItem(QPixmap(":/item/src/item/createSceneTitle.png"));
    title->setScale(Settings::startSceneTitleScale);
    //title x置中 y设高度50
    title->setPos((Settings::screenWidth - title->sceneBoundingRect().width())/2,
                  Settings::startSceneTitleY);
    addItem(title);

    //functional btns
    settingBtn = new FunctionBtn(QPixmap(":/item/src/item/settingIcon.png"));
    backBtn = new FunctionBtn(QPixmap(":/item/src/item/backIcon.png"));

    //功能按键列的位置排版
    settingBtn->setPos(Settings::screenWidth - (settingBtn->boundingRect().width() + Settings::functionBtnInterval),
                       Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    backBtn->setPos(Settings::screenWidth - 2*(Settings::functionBtnInterval + settingBtn->sceneBoundingRect().width()),
                          Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);


    addItem(settingBtn);
    addItem(backBtn);

    levelblock = new FunctionBtn(QPixmap(":/item/src/item/myLevelBlock.png"));
    buildblock = new FunctionBtn(QPixmap(":/item/src/item/buildblock.png"));
    levelblock->setPos(Settings::screenWidth/2 - 3/2*levelblock->boundingRect().width(),(Settings::screenHeight - levelblock->boundingRect().height())/2);
    buildblock->setPos(Settings::screenWidth/2 + levelblock->boundingRect().width()/2,(Settings::screenHeight - levelblock->boundingRect().height())/2);
    addItem(levelblock);
    addItem(buildblock);
}


CreateLevelScene::CreateLevelScene(QObject *parent)
    :Scene{parent}
{
    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/ingame_background1.png"));
    addItem(bg);

    //functional btns
    settingBtn = new FunctionBtn(QPixmap(":/item/src/item/settingIcon.png"),bg);
    shareBtn = new FunctionBtn(QPixmap(":/item/src/item/shareIcon.png"),bg);

    settingBtn->setPos(Settings::functionBtnInterval,
                       Settings::screenHeight - settingBtn->boundingRect().height() - 5);

    shareBtn->setPos(2*Settings::functionBtnInterval + settingBtn->boundingRect().width(),
                    Settings::screenHeight - settingBtn->boundingRect().height() - 5);

    //加入textblock
    content = new QGraphicsTextItem("content:",bg);
    title =  new QGraphicsTextItem("title:",bg);
    title->setPos(QPointF(100,100));
    content->setPos(QPointF(100,200));
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",title,Qt::black,20);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",content,Qt::black,20);

    content_edit = new QTextEdit;
    content_edit->setPlaceholderText("输入关卡内容...");
    title_edit = new QTextEdit;
    title_edit->setPlaceholderText("输入关卡标题...");

    content_edit->setFixedSize(400,400);
    title_edit->setFixedSize(250,30);
    content_edit->move(content->x() + 150,content->y());
    title_edit->move(content_edit->x(),title->y());

    addWidget(title_edit);
    addWidget(content_edit);

}

QString CreateLevelScene::getContent()
{
    return content_edit->toPlainText();
}

QString CreateLevelScene::getTitle()
{
    return title_edit->toPlainText();
}

void CreateLevelScene::setDifficulty(int h)
{
    difficulty = h;
}

void CreateLevelScene::setMode(int m)
{
    mode = m;
}

QString CreateLevelScene::getWholeText()
{
    QString l1 = "title=" + getTitle() + "\n";
    QString temp,temp2;
    if(mode == CLASSIC){
        temp = "classic";
        temp2 = QString::number(difficulty) + ',' + QString::number(difficulty);
    }else{
        temp = "hex";
        temp2 = QString::number(difficulty);
    }

    QString l2 = "mode=" + temp + "\n";
    QString l3 = "difficulty=" + temp2 + '\n';

    QString l4 = "content=\n";
    QString l5;
    if(mode == CLASSIC){

        if(getContent().size() < difficulty*difficulty){
            QMessageBox::information(nullptr,"内容字数不够！","CLassic模式至少需要" + QString::number(difficulty*difficulty) + "个字符！");
            return "";
        }

        int index = 0;
        for (int j = 0; j < difficulty; ++j) {
            for (int i = 0; i < difficulty; ++i) {
                l5.append(getContent()[index]);
                ++index;
            }
            l5 += '\n';
        }

    }
    else{

        if(getContent().size() < 3*difficulty*(difficulty-1)+1){
            QMessageBox::information(nullptr,"内容字数不够！","至少需要" + QString::number(3*difficulty*(difficulty-1)+1) + "个字符！");
            return "";
        }

        int wordNum = difficulty;
        int index = 0;
        for (int line = 1; line <= 2*difficulty-1; ++line) {

            for (int i = index; i < index+wordNum; ++i) {
                l5.append(getContent()[i]);
            }

            index += wordNum;
            l5 += '\n';

            //前半部分，每行使wordNum++
            if(line < difficulty){
                ++wordNum;
            }
            //后半部分每行--
            else{
                --wordNum;
            }
        }
    };

    return QString(l1+l2+l3+l4+l5);


}
