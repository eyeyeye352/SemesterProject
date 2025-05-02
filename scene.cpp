#include "scene.h"
#include "objpool.h"


Scene::Scene(QObject *parent)
    : QGraphicsScene{parent}
{
    setSceneRect(QRectF(0,0,Settings::screenWidth,Settings::screenHeight));
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

    //功能按键列的位置排版
    settingBtn->setPos(Settings::functionBtnInterval,
                       Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    rankBtn->setPos(2*Settings::functionBtnInterval + settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    createModeBtn->setPos(3*Settings::functionBtnInterval + 2*settingBtn->sceneBoundingRect().width(),
                          Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    backBtn->setPos(4*Settings::functionBtnInterval + 3*settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    //backBtn初始不可见。
    backBtn->setOpacity(0);

    addItem(settingBtn);
    addItem(rankBtn);
    addItem(createModeBtn);
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
    QGraphicsPixmapItem * bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/ingame_background1.png"));
    addItem(bg);

    //functional btns
    settingBtn = new FunctionBtn(QPixmap(":/item/src/item/settingIcon.png"));
    rankBtn = new FunctionBtn(QPixmap(":/item/src/item/rankIcon.png"));

    /* developing
     *
     * saveBtn = new FunctionBtn(QPixmap(""));
     * saveBtn->setPos(15 + 2*settingBtn->sceneBoundingRect().width() ,Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);
     * saveBtn->setOpacity(Settings::funcBtnOriginOpacity);
     * addItem(saveBtn);
     *
    */

    //功能按键
    settingBtn->setPos(Settings::functionBtnInterval,
                       Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    rankBtn->setPos(2*Settings::functionBtnInterval + settingBtn->sceneBoundingRect().width(),
                    Settings::screenHeight - settingBtn->sceneBoundingRect().height() - 5);

    addItem(settingBtn);
    addItem(rankBtn);


    //title & font
    title = new QGraphicsTextItem;

    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,25);
    title->setFont(font);
    title->setDefaultTextColor(Qt::white);
    title->setPos((Settings::screenWidth - title->sceneBoundingRect().width())/2,
                  Settings::levelSceneTiTleY);
    addItem(title);

}

void LevelScene::loadLevel(QString filepath)
{
    release();

    QFile file(filepath);
    file.open(QIODevice::ReadOnly);

    //qDebug() << "fileopen:" << file.isOpen();

    //处理字符串
    QString whole = file.readAll();
    QStringList strList = whole.split("\r\n");

    //qDebug() << whole;

    //字符串处理
    for (int line = 0; line < strList.size(); ++line) {

        //title部分
        if(strList[line].startsWith("title=")){
            title->setPlainText(strList[line].remove("title="));
        }

        //难度（行列数）部分
        else if(strList[line].startsWith("difficulty=")){

            //确保格式错误时不会崩溃 ,标准格式："数字,数字"
            QRegularExpression regex("^(\\d+),(\\d+)$");
            QString nums = strList[line].remove("difficulty=");
            QRegularExpressionMatch match = regex.match(nums);


            if (match.hasMatch()) {
                rows = match.captured(1).toInt();
                cols = match.captured(2).toInt();
            } else {
                rows = 0;
                cols = 0;
            }


        }

        //模式
        else if(strList[line].startsWith("mode=")){
            QString t = strList[line].remove("mode=");
            if(t == "classic") mode = CLASSIC;
            else if(t == "hex") mode = HEX;
        }

        //contents内容
        else if(strList[line].startsWith("content=")){
            for (int y = line+1; y <= line+cols && y < strList.size(); ++y) {
                contents.append(strList[y]);
            }
            break;
        }
    }

    //debug信息
    qDebug() << "content" << contents;

    //textblocks渲染
    double Width = rows * Settings::textBlockSize;
    double Height = cols * Settings::textBlockSize;

    for (int y = 0; y < cols; ++y) {
        for (int x = 0; x < rows; ++x) {

            //每个textBlock储存其位置[（0,0）~(rows,cols)]与一个汉字
            //目标汉字位于 contents的 y*每行字数 + x
            QString word = contents[y*rows + x];
            TextBlock * block = objPool::getinstance()->getTextBlock();
            block->setWord(word);
            block->setxy({x,y});

            qDebug() << "testdebug(block setpos and word)";

            //排版置中
            double posx = (Settings::screenWidth - Width)/2 + x*Settings::textBlockSize;
            double posy = (Settings::screenHeight - Height)/2 + y*Settings::textBlockSize;
            block->setPos(posx,posy);

            textBlocks.append(block);
            addItem(block);
        }
    }

    qDebug() << "loadfinish";

    file.close();
}

void LevelScene::release()
{
    contents = "";

    for (int n = 0; n < textBlocks.size(); ++n) {
        objPool::getinstance()->recycle(textBlocks[n]);
        removeItem(textBlocks[n]);
    }

    textBlocks.clear();
    qDebug() << "release success.";

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
    musicSli->setPos(Settings::settingPage_MusicValSet);
    soundSli->setPos(Settings::settingPage_SoundValSet);

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
    backHomeBtn->setOpacity(1);
    closeSettingBtn->setOpacity(1);


    backHomeBtn->setPos((bg->boundingRect().width() - backHomeBtn->boundingRect().width())/2,
                        bg->boundingRect().height() - backHomeBtn->boundingRect().height() - 25);

    closeSettingBtn->setPos({25,25});


    QObject::connect(backHomeBtn,&GameBtn::clicked,[this]{ emit backHome();});
    QObject::connect(closeSettingBtn,&GameBtn::clicked,[this]{ emit closeSetting(); });

}
