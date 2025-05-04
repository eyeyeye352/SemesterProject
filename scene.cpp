#include "scene.h"
#include "objpool.h"


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
    :Scene{parent},
    hasSelectBlocks(false),
    curTransType(TranslateIcons::NONE)
{
    QGraphicsPixmapItem * bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/ingame_background1.png"));
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
    connect(sideBar,&MySideBar::SelectTransType,this,&LevelScene::changeTransType);
    addItem(sideBar);


    //title
    title = new QGraphicsTextItem(sideBar);

    //font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,18);
    title->setFont(font);
    title->setDefaultTextColor(Qt::white);
    addItem(title);

}


void LevelScene::loadLevel(QString filepath)
{
    reset();

    QFile file(filepath);
    file.open(QIODevice::ReadOnly);

    //qDebug() << "fileopen:" << file.isOpen();

    //处理字符串
    QString whole = file.readAll();
    QStringList strList = whole.split("\r\n");

    for (int line = 0; line < strList.size(); ++line) {

        //title部分
        if(strList[line].startsWith("title=")){
            title->setPlainText(strList[line].remove("title="));
            title->setPos((sideBar->boundingRect().width() - title->boundingRect().width())/2 + 10 , 600);
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
            setGameMode(mode);
        }

        //contents内容
        else if(strList[line].startsWith("content=")){
            for (int y = line+1; y <= line+rows && y < strList.size(); ++y) {
                contents.append(strList[y]);
            }
            break;
        }
    }

    //debug信息
    qDebug() << "content" << contents;

    //textblocks渲染
    double Width = cols * Settings::textBlockSize;
    double Height = rows * Settings::textBlockSize;

    /*
     * 置中排版：
     * double Width = rows * Settings::textBlockSize;
     * double Height = cols * Settings::textBlockSize;
     * double posx = (Settings::screenWidth - Width)/2 + x*Settings::textBlockSize;
     * double posy = (Settings::screenHeight - Height)/2 + y*Settings::textBlockSize;
     *
     *
     * 左上角排版：
     * double startX = Settings::textBlockSize;
     * double startY = startX;
     * double posx = startX + x*Settings::textBlockSize;
     * double posy = startY + y*Settings::textBlockSize;
    */

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {

            //每个textBlock储存其位置[左上角：（0,0）~ 右下角 (cols,rows)]与一个汉字
            //目标汉字位于 contents的 y*每row字数 (即y*cols) + x
            QString word = contents[y*cols + x];
            TextBlock * block = objPool::getinstance()->getTextBlock();
            block->setWord(word);
            block->setxy({x,y});


            double posx = (Settings::screenWidth - Width)/2 + x*Settings::textBlockSize;
            double posy = (Settings::screenHeight - Height)/2 + y*Settings::textBlockSize;
            block->setPos(posx,posy);

            textBlocks.append(block);
            addItem(block);

            //链接(防止重复链接)
            disconnect(block, &TextBlock::clicked, nullptr, nullptr);
            connect(block,&TextBlock::clicked,[this](TextBlock* t){
                hasSelectBlocks ? switchBlocks(t) : selectBlocks(t);
            });
            sideBar->setZValue(block->zValue() + 1);
        }
    }

    // qDebug() << "loadfinish";
    // for (TextBlock* t : textBlocks) {
    //     qDebug() << t->Word() << " 位于 " << t->getXY();
    // }

    file.close();
    sideBar->setZValue(maxZvalue() + 1);
}

void LevelScene::reset()
{
    contents = "";
    curTransType = TranslateIcons::NONE;
    cancelSelect();
    gameRecords.clear();

    for (int n = 0; n < textBlocks.size(); ++n) {
        objPool::getinstance()->recycle(textBlocks[n]);
        removeItem(textBlocks[n]);
    }

    textBlocks.clear();
    qDebug() << "reset success.";



}



void LevelScene::changeTransType(TranslateIcons::Type type)
{
    if(curTransType != type){
        curTransType = type;
        qDebug() << "currentType:" << curTransType;
    }

    cancelSelect();

}

void LevelScene::selectBlocks(TextBlock* target)
{

    switch(curTransType){

    case TranslateIcons::ROWS:
        //row select
        rowSelect(target);
        break;
    case TranslateIcons::COLS:
        //col select
        colSelect(target);
        break;
    case TranslateIcons::CROSS:
        //cross select
        crossSelect(target);
        break;
    case TranslateIcons::HEXLINE:
        break;
    case TranslateIcons::HEX:
        break;
    }

    if(!selectedBlocks.isEmpty() || !crossTransMap.isEmpty()){
        hasSelectBlocks = true;
    }
}


void LevelScene::rowSelect(TextBlock * target)
{
    for (TextBlock* t : textBlocks) {
        if(t->getXY().y() == target->getXY().y()){
            t->showGoldRect();
            selectedBlocks.push_back(t);
        }
    }
}

void LevelScene::colSelect(TextBlock *target)
{
    for (TextBlock* t : textBlocks) {
        if(t->getXY().x() == target->getXY().x()){
            t->showGoldRect();
            selectedBlocks.push_back(t);
        }
    }
}

void LevelScene::crossSelect(TextBlock *target)
{
    //xy 不能为边缘地区
    if(target->getXY().x() == 0 || target->getXY().x() == cols-1
        || target->getXY().y() == 0 || target->getXY().y() == rows-1)
    {
        qDebug() << "无效选取";
        return;
    }

    for (TextBlock* t : textBlocks){
        //t == up
        if(t->getXY() - target->getXY() == QPoint(0,-1)){
            crossTransMap.insert("UP",t);
        }
        //t == down
        else if(t->getXY() - target->getXY() == QPoint(0,1)){
            crossTransMap.insert("DOWN",t);
        }
        //t == left
        else if(t->getXY() - target->getXY() == QPoint(-1,0)){
            crossTransMap.insert("LEFT",t);
        }
        //t == right
        else if(t->getXY() - target->getXY() == QPoint(1,0)){
            crossTransMap.insert("RIGHT",t);
        }
    }
    crossTransMap.insert("CENTER",target);

    //加金色框框
    for (TextBlock* t : crossTransMap) {
        t->showGoldRect();
    }

}

//取消选中
void LevelScene::cancelSelect()
{
    hasSelectBlocks = false;

    for (TextBlock* t : selectedBlocks) {
        t->hideGoldRect();
    }
    selectedBlocks.clear();

    for (TextBlock* t : crossTransMap) {
        t->hideGoldRect();
    }
    crossTransMap.clear();
}


//已经有选中的方块时，在次点击方块进行交换
void LevelScene::switchBlocks(TextBlock* dest)
{

    switch(curTransType){

    case TranslateIcons::ROWS:
        rowSwitch(dest);
        break;
    case TranslateIcons::COLS:
        colSwitch(dest);
        break;
    case TranslateIcons::CROSS:
        crossSwitch(dest);
        break;
    case TranslateIcons::HEXLINE:
        break;
    case TranslateIcons::HEX:
        break;
    }

    cancelSelect();
}


void LevelScene::rowSwitch(TextBlock* dest)
{
    QList<TextBlock*> blocksForSwitch;
    for (TextBlock* t : textBlocks){
        if(t->getXY().y() == dest->getXY().y()){
            blocksForSwitch.push_back(t);
        }
    }

    for (int x = 0; x < cols; ++x) {
        TextBlock::switchWord(blocksForSwitch[x],selectedBlocks[x]);
    }

    MusicPlayer::getMPlayer()->playBlockSound(true);
}

void LevelScene::colSwitch(TextBlock* dest)
{
    QList<TextBlock*> blocksForSwitch;
    for (TextBlock* t : textBlocks){
        if(t->getXY().x() == dest->getXY().x()){
            blocksForSwitch.push_back(t);
        }
    }

    for (int y = 0; y < rows; ++y) {
        TextBlock::switchWord(blocksForSwitch[y],selectedBlocks[y]);
    }

    MusicPlayer::getMPlayer()->playBlockSound(true);
}

void LevelScene::crossSwitch(TextBlock* dest)
{

    //dest 不能为边缘地区
    if(dest->getXY().x() == 0 || dest->getXY().x() == cols-1
        || dest->getXY().y() == 0 || dest->getXY().y() == rows-1)
    {
        qDebug() << "不能选择边缘";
        MusicPlayer::getMPlayer()->playBlockSound(false);
        return;
    }

    //不能有重叠区块
    if((dest->getXY() - crossTransMap["CENTER"]->getXY()).manhattanLength() <= 2){
        qDebug() << "不能有重叠区域";
        MusicPlayer::getMPlayer()->playBlockSound(false);
        return;
    }


    for (TextBlock* t : textBlocks){
        //t == dest-up
        if(t->getXY() - dest->getXY() == QPoint(0,-1)){
            TextBlock::switchWord(t,crossTransMap["UP"]);
        }
        //t == dest-down
        else if(t->getXY() - dest->getXY() == QPoint(0,1)){
            TextBlock::switchWord(t,crossTransMap["DOWN"]);
        }
        //t == dest-left
        else if(t->getXY() - dest->getXY() == QPoint(-1,0)){
            TextBlock::switchWord(t,crossTransMap["LEFT"]);
        }
        //t == dest-right
        else if(t->getXY() - dest->getXY() == QPoint(1,0)){
            TextBlock::switchWord(t,crossTransMap["RIGHT"]);
        }
    }

    TextBlock::switchWord(dest,crossTransMap["CENTER"]);
    MusicPlayer::getMPlayer()->playBlockSound(true);
}



void LevelScene::setGameMode(Mode m)
{
    mode = m;
    reset();
    sideBar->setGameMode(mode);
}

QString LevelScene::getLevelContent()
{
    return contents;
}

int LevelScene::getCols()
{
    return cols;
}
int LevelScene::getRows()
{
    return rows;
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


TipPage::TipPage(QObject *parent):
    Scene(parent)
{
    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/tipPage.png"));
    addItem(bg);

    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);

    answer = new QGraphicsTextItem(bg);
    //font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/tipPageTextFont.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,17);
    answer->setFont(font);
    answer->setDefaultTextColor(Qt::white);
    addItem(answer);

    //btn
    closeTipBtn = new FunctionBtn(QPixmap(":/item/src/item/closeSettingBtn.png"),bg);
    closeTipBtn->setPos(25,25);
    addItem(closeTipBtn);
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
