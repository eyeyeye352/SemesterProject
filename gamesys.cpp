#include "gamesys.h"
#include "animation.h"
#include "myalgorithms.h"


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

    //other scene
    settingPage = new SettingPage(this);
    tipPage = new TipPage(this);
    completePage = new CompletePage(this);
    savePage = new SavePage(this);
    rankPage = new RankPage(this);


    //timers
    bgmoveTimer = new QTimer(this);
    bgmoveTimer->start(Settings::backgroundUpdateInterval);
    connect(bgmoveTimer,&QTimer::timeout,startscene,&StartScene::moveBG);

    //bgm
    MusicPlayer::getMPlayer()->startBGM();

    //startScene connections
    connect(startscene->classicBtn,&ClassicBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->hexBtn,&HexBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->settingBtn,&GameBtn::clicked,this,&Gamesys::openSettingPage);
    connect(startscene->rankBtn,&GameBtn::clicked,this,&Gamesys::openRankPage);
    connect(startscene->createModeBtn,&GameBtn::clicked,this,&Gamesys::goCreateMode);
    connect(startscene->backBtn,&GameBtn::clicked,this,&Gamesys::backModeSelection);
    connect(startscene->saveBtn,&GameBtn::clicked,this,&Gamesys::openSavePage);

    for (int n = 0; n < startscene->levels.size(); ++n) {
        connect(startscene->levels[n],&LevelSelectBlock::selected,[this](int levelNum){
            loadGameAnime(levelNum,true);
        });
    }

    //connections(levelScene)
    connect(levelscene->settingBtn,&GameBtn::clicked,this,&Gamesys::openSettingPage);
    connect(levelscene->rankBtn,&GameBtn::clicked,this,&Gamesys::openRankPage);
    connect(levelscene->saveBtn,&GameBtn::clicked,this,&Gamesys::openSavePage);
    connect(levelscene->tipBtn,&GameBtn::clicked,this,&Gamesys::openTipPage);
    connect(levelscene->undoBtn,&GameBtn::clicked,this,&Gamesys::UndoTrans);
    connect(levelscene->doBtn,&GameBtn::clicked,this,&Gamesys::DoTrans);
    connect(levelscene->sideBar,&MySideBar::SelectTransType,this,&Gamesys::changeTransType);
    connect(levelscene,&LevelScene::clickBg,this,&Gamesys::cancelSelect);

    //connection(otherpage)
    connect(settingPage,&SettingPage::goHome,this,&Gamesys::goHome);
    connect(settingPage,&TempPage::closePage,this,&Gamesys::closeTempPage);

    connect(tipPage,&TempPage::closePage,this,&Gamesys::closeTempPage);

    connect(completePage,&CompletePage::goHome,this,&Gamesys::goHome);

    connect(savePage,&TempPage::closePage,this,&Gamesys::closeTempPage);
    connect(savePage,&SavePage::slotSelected,this,&Gamesys::processSlots);

    initSLSlot();

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
    levelscene->setGameMode(currentMode);

    qDebug() << "play mode:" << currentMode;
}


void Gamesys::backModeSelection()
{
    Animation::backModeSelection(startscene);
}


//子功能页面 settingPage、tipPage、rankPage、savePage
void Gamesys::openSettingPage()
{
    tempview->setScene(settingPage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);
}

void Gamesys::openTipPage()
{
    tempview->setScene(tipPage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);
}

void Gamesys::openSavePage()
{
    if(view->scene() == startscene){
        savePage->hideSaveBtn();
    }else if(view->scene() == levelscene){
        savePage->showSaveBtn();
    }

    tempview->setScene(savePage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);
}

void Gamesys::openRankPage()
{

}

//设置页面返回首页
void Gamesys::goHome()
{

    if(view->scene() == startscene){
        closeTempPage();
    }

    //关闭tempview动画结束后，返回首页
    else if(view->scene() == levelscene){

        // for (int n = 0 ; n < textBlocks.size() ; ++n) {
        //     objPool::getinstance()->recycle(textBlocks[n]);
        //     levelscene->removeItem(textBlocks[n]);
        // }

        QPropertyAnimation* anime = Animation::TempPageout(view,tempview);

        //设置页面退出动画
        connect(anime,&QPropertyAnimation::finished,[this]{
            Animation::changeScene(levelscene,startscene,view,1500)->start(QAbstractAnimation::DeleteWhenStopped);
            MusicPlayer::getMPlayer()->changeBgm(Settings::bgmList["pastel_sublinal"]);
            tempview->setScene(nullptr);
        });

        anime->start(QAbstractAnimation::DeleteWhenStopped);
    }

    resetLevel();
}



void Gamesys::closeTempPage()
{
    QPropertyAnimation* anime = Animation::TempPageout(view,tempview);
    anime->start(QAbstractAnimation::DeleteWhenStopped);

    connect(anime,&QPropertyAnimation::finished,[this]{
        tempview->setScene(nullptr);
    });
}





void Gamesys::goCreateMode()
{

}

//切换音乐和scene,然后在loadingscene动画中加载游戏
void Gamesys::loadGameAnime(int levelNum,bool shuffled)
{

    MusicPlayer::getMPlayer()->changeBgm(Settings::bgmList["InGame_classicMode"]);
    QSequentialAnimationGroup* anime = Animation::changeScene(startscene,levelscene,view,3000);
    connect(anime->animationAt(1),&QPropertyAnimation::finished,[=]{
        //动画暂停时加载文件
        loadGame(levelNum,shuffled);
    });
    anime->start(QAbstractAnimation::DeleteWhenStopped);
}


//加载文件

//hex文本格式
//total hexblock num = 3*radius(radius-1)+1
//text form:
/*
 * radius,
 * radius + 2,
 * radius + 4,
 * ...
 * radius + (2n-2)
 * ...
 * radius + 4,
 * radius + 2,
 * radius.
 *
*/
void Gamesys::loadGame(int levelNum,bool shuffled)
{

    //根据模式选择关卡文件
    QString filepath = QString(":/Levels/src/levelsDoc/ClassicL%1.txt").arg(levelNum);
    switch(currentMode){
    case CLASSIC:
        filepath = QString(":/Levels/src/levelsDoc/ClassicL%1.txt").arg(levelNum);
        break;
    case HEX:
        filepath = QString(":/Levels/src/levelsDoc/HexL%1.txt").arg(levelNum);
        break;
    }

    //加载前先重置各种成员
    resetLevel();

    //处理字符串
    QString whole = MyAlgorithms::getContentInFile(filepath);
    QStringList strList = whole.split("\r\n");

    for (int line = 0; line < strList.size(); ++line) {

        //title部分
        if(strList[line].startsWith("title=")){
            levelscene->setTitle(strList[line].remove("title="));
        }


        //模式
        else if(strList[line].startsWith("mode=")){

            QString t = strList[line].remove("mode=");

            if(t == "classic") currentMode = CLASSIC;
            else if(t == "hex") currentMode = HEX;
            levelscene->setGameMode(currentMode);
        }

        //处理难度部分
        else if(strList[line].startsWith("difficulty=")){
            readDifficulty(strList[line]);
        }



        //contents内容
        else if(strList[line].startsWith("content=")){

            int contentLines;

            if(currentMode == CLASSIC){
                contentLines = rows;
            }else if(currentMode == HEX){
                contentLines = 2*radius - 1;
            }

            for (int n = line+1; n <= line+contentLines && n < strList.size(); ++n) {
                contents.append(strList[n]);
            }
            break;
        }
    }

    //debug信息
    qDebug() << "content read" << contents;

    loadTextBlocks();

    // for (int y = 0; y < rows; ++y) {
    //     for (int x = 0; x < cols; ++x) {

    //         //每个textBlock储存其位置[左上角：（0,0）~ 右下角 (cols,rows)]与一个汉字
    //         //目标汉字位于 contents的 y*每row字数 (即y*cols) + x
    //         QString word = contents[y*cols + x];
    //         TextBlock * t = objPool::getinstance()->getTextBlock();
    //         t->setWord(word);
    //         t->setxy({x,y});
    //         textBlocks.append(t);

    //         //每个方块链接信号
    //         disconnect(t, &TextBlock::clicked, nullptr, nullptr);
    //         connect(t,&TextBlock::clicked,this,[this](TextBlock* t){
    //             hasSelects ? switchBlocks(t) : selectBlocks(t);
    //         },Qt::UniqueConnection);

    //         //方块交给levelscene渲染
    //         levelscene->addTextBlock(t,rows,cols);
    //     }
    // }

    qDebug() << "loading finish.";

    /*
    for (TextBlock* t : textBlocks) {
        qDebug() << t->Word() << "has loaded in" << t->getXY();
    }
    */

    if(currentMode == CLASSIC){
        tipPage->setClassicAnswer(contents,cols);
    }else if(currentMode == HEX){
        tipPage->setHexAnswer(contents,radius);
    }


    curLevelNum = levelNum;

    //有洗牌表示从startscene选关进入，没洗牌表示载入存档
    if(shuffled){
        shuffleLevel();
    }
}



void Gamesys::readDifficulty(QString d_line)
{
    if(currentMode == CLASSIC){
        //regexp确保格式匹配"数字,数字"
        QRegularExpression regex("^(\\d+),(\\d+)$");
        QString nums = d_line.remove("difficulty=");
        QRegularExpressionMatch match = regex.match(nums);

        if (match.hasMatch()) {
            rows = match.captured(1).toInt();
            cols = match.captured(2).toInt();
        } else {
            rows = 0;
            cols = 0;
        }

        qDebug() << "rows,cols be read:" << rows << "," << cols;
    }
    else if(currentMode == HEX){
        radius = d_line.remove("difficulty=").toInt();
        qDebug() << "radius(difficulty) be read:" << radius;
    }

}

void Gamesys::loadTextBlocks()
{

    //临时指针，指向中心hex
    HexTextBlock* center;

    if(currentMode == CLASSIC){
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {

                //每个textBlock储存其位置[左上角：（0,0）~ 右下角 (cols,rows)]与一个汉字
                //目标汉字位于 contents的 y*每row字数 (即y*cols) + x
                QString word = contents[y*cols + x];
                RectTextBlock * t = objPool::getinstance()->getRectTextBlock();
                t->setWord(word);
                t->setxy({x,y});
                textBlocks.append(t);

                //每个方块链接信号
                disconnect(t, &RectTextBlock::clicked, nullptr, nullptr);
                connect(t,&RectTextBlock::clicked,this,[this](RectTextBlock* t){
                    hasSelects ? switchBlocks(t) : selectBlocks(t);
                },Qt::UniqueConnection);

                //方块交给levelscene渲染
                levelscene->addTextBlock(t,rows,cols);
            }
        }
    }

    else if(currentMode == HEX){


        //中间行有 2n - 1 个方块。
        //
        //全部共有 3n(n-1)+1个小六边形。

        //共有 2*radius - 1 个横排。
        QList<HPoint> hexCoordis = MyAlgorithms::makeHexCoordi(radius);

        for (int N = 0; N < 3*radius*(radius-1)+1; ++N) {


        QString word = contents[N];
        HexTextBlock * h = objPool::getinstance()->getHexTextBlock();
        h->setWord(word);
        h->setHpos(hexCoordis[N]);
        textBlocks.append(h);

        //每个方块链接信号
        disconnect(h, &HexTextBlock::clicked, nullptr, nullptr);
        connect(h,&HexTextBlock::clicked,this,[this](HexTextBlock* h){
                    hasSelects ? switchBlocks(h) : selectBlocks(h);
        },Qt::UniqueConnection);
        levelscene->addTextBlock(h);

        //定位中心
        if(h->getXY() == QPoint(0,0)){
            center = h;
        }

    }

    //hex添加邻居关系
    if(currentMode == HEX){
        for (int n = 0; n < textBlocks.size(); ++n) {
            for (int nn = 0; nn < textBlocks.size(); ++nn) {
                QPoint dis = textBlocks[nn]->getXY() - textBlocks[n]->getXY();
                if(dis == QPoint(1,0) || dis == QPoint(1,1)
                    || dis == QPoint(0,1) || dis == QPoint(-1,0)
                    || dis == QPoint(-1,-1) || dis == QPoint(0,-1))
                {
                    static_cast<HexTextBlock*>(textBlocks[n])->addNeightBor(textBlocks[nn]);
                }
            }
        }
    }

    center->setThisAsCenter();
}










//初始加载slot信息。
void Gamesys::initSLSlot()
{

    // 获取用户文档目录，并创建一个名为 /gameSavings 的子目录
    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/gameSavings";
    QDir().mkpath(saveDir); // 创建保存目录，如果不存在

    for (int n = 0; n < savePage->getSlots().size(); ++n) {
        QString filepath = QDir(saveDir).filePath("s%1.txt");
        qDebug() << "init slots by:" << filepath.arg(QString::number(n));
        savePage->getSlots()[n]->init(filepath.arg(QString::number(n)));
    }

}

void Gamesys::processSlots(int slotNum, int state)
{
    if(state == SavePage::NONE){
        //show slot info and do nothing.
        savePage->getSlots()[slotNum]->info();
    }
    else if(state == SavePage::SAVE){
        //developing
        saveSlotAt(slotNum);
        initSLSlot();
        savePage->backToNoneState();

        qDebug() << "存档系统记录：" << sysRecord.size() << "玩家记录：" << playerRecords.size();
    }
    else if(state == SavePage::LOAD){
        //loading
        loadSaveGame(savePage->getSlots()[slotNum]);
        //复原savePage
        savePage->backToNoneState();
    }

}


void Gamesys::loadSaveGame(SaveSlot* S)
{
    //检测是否为有效存档
    if(S->getSysRecords().isEmpty()){
        qDebug() << "无效的存档";
        return;
    }


    //获取各种属性
    curLevelNum = S->getLevelNum();
    currentMode = S->getGameMode();

    //debug
    qDebug() << "curmode after loadSL:" << currentMode;
    qDebug() << "curlevelnum after SL:" << curLevelNum;


    //先关闭savePage，切换场景(关卡)、不洗牌。
    closeTempPage();

    if(view->scene() == startscene){
        MusicPlayer::getMPlayer()->changeBgm(Settings::bgmList["InGame_classicMode"]);
    }

    QSequentialAnimationGroup* anime = Animation::changeScene(static_cast<Scene*>(view->scene()),levelscene,view,3500);
    connect(anime->animationAt(1),&QPropertyAnimation::finished,[=]{

        //注意从loadscene到loadscene时要重置关卡


        //动画暂停时加载文件(注意加载时会把Record刷掉)
        loadGame(curLevelNum,false);

        for (int n = 0; n < textBlocks.size(); ++n) {
            qDebug() << textBlocks[n]->getWord() << " loaded at" << textBlocks[n]->getXY();
        }

        //载入存档记录
        sysRecord = S->getSysRecords();
        playerRecords = S->getPlayerRecords();

        for (int n = 0; n < sysRecord.size(); ++n) {
            GameRecord r = sysRecord[n];
            qDebug() << "载入sys记录：";
            r.info();
            //bug点
            selectAndSwitch(r.type,findBlockAt(r.startXY),findBlockAt(r.toXY));
        }

        if(playerRecords.size() > 0){
            levelscene->undoBtn->setOpacity(1);
        }
        for (int n = 0; n < playerRecords.size(); ++n) {
            GameRecord r = playerRecords[n];
            qDebug() << "载入player记录：";
            r.info();
            selectAndSwitch(r.type,findBlockAt(r.startXY),findBlockAt(r.toXY));
        }

        //设置步数
        useStep = playerRecords.size();
        levelscene->setStep(useStep);
    });
    anime->start(QAbstractAnimation::DeleteWhenStopped);
/*
    for (int n = 0; n < textBlocks.size(); ++n) {
        qDebug() << "textblock:" << textBlocks[n]->Word() << textBlocks[n]->getXY();
    }
*/
}


//存档
void Gamesys::saveSlotAt(int slotNum)
{

    // 获取用户文档目录，并创建一个名为 /gameSavings 的子目录
    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/gameSavings";
    QDir().mkpath(saveDir); // 创建保存目录，如果不存在

    QString filepath = saveDir + "/s%1.txt";
    QFile file(filepath.arg(slotNum));
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    QString origtext = MyAlgorithms::saveTextOutPut;

    //savetime
    QDateTime now = QDateTime::currentDateTime();
    QString savetime = now.toString("yyyy-MM-dd hh:mm:ss");

    //mode
    QString mode = QString::number(currentMode);

    //levelnum
    QString levelNumstr = QString::number(curLevelNum);

    //record amount
    QString rNum = QString::number(sysRecord.size() + playerRecords.size());

    QStringList gamerecords;

    //sys record
    for (int n = 0; n < sysRecord.size(); ++n) {
        //sys:(transtype) (startx) (starty) (destx) (desty)
        gamerecords.append("sys:" + QString::number(sysRecord[n].type) + ' ' +
                           QString::number(sysRecord[n].startXY.x()) + ' ' +
                           QString::number(sysRecord[n].startXY.y()) + ' ' +
                           QString::number(sysRecord[n].toXY.x()) + ' ' +
                           QString::number(sysRecord[n].toXY.y()) + '\n');
    }
    //player record
    for (int n = 0; n < playerRecords.size(); ++n) {
        //sys:(transtype) (startx) (starty) (destx) (desty)
        gamerecords.append("player:" + QString::number(playerRecords[n].type) + ' ' +
                           QString::number(playerRecords[n].startXY.x()) + ' ' +
                           QString::number(playerRecords[n].startXY.y()) + ' ' +
                           QString::number(playerRecords[n].toXY.x()) + ' ' +
                           QString::number(playerRecords[n].toXY.y()) + '\n');
    }

    //output
    QString finaltext = origtext.arg(savetime).arg(mode).arg(levelNumstr).arg(rNum);
    for (int n = 0; n < gamerecords.size(); ++n) {
        finaltext.append(gamerecords[n]);
    }
    out << finaltext;

    qDebug() << "saveat:" << file.fileName();
    file.close();
}



//载入关卡时的洗牌算法,成功变换时添加进systrans记录表
void Gamesys::shuffleLevel()
{
    //变换次数随关卡数字提升
    int transtimes = curLevelNum*5;


    //变换模式列表
    QList<TranslateIcons::Type> types =
        {
            TranslateIcons::ROWS,
            TranslateIcons::COLS,
            TranslateIcons::CROSS,
            TranslateIcons::ROTATE,
            TranslateIcons::HEX
        };


    //变换直到sysrecord有效记录次数达到指定次数
    while(sysRecord.size() < transtimes){

        int index;
        //随机选一种变换方式（classic关卡为1，3时不进行十字变换）
        if(currentMode == CLASSIC && (curLevelNum == 1 || curLevelNum == 3)){
            index = QRandomGenerator::global()->bounded(0,2);
        }
        else if(currentMode == CLASSIC){
            index = QRandomGenerator::global()->bounded(0,3);
        }
        //hex模式
        else if(currentMode == HEX){
            index = QRandomGenerator::global()->bounded(3,5);
            //先退出，还未开发
            break;
        }

        //qDebug() << "洗牌模式：" << index;


        //随机选两个textblock
        TextBlock* start = nullptr , *dest = nullptr;

        switch(types[index]){

        case TranslateIcons::ROWS:

            //优化：确保变换不在同一行
            do{
                start = getRandBlock();
                dest = getRandBlock();
            }while(start->getXY().y() == dest->getXY().y());
            break;

        case TranslateIcons::COLS:


            //优化：确保变换不在同一列
            do{
                start = getRandBlock();
                dest = getRandBlock();
            }while(start->getXY().x() == dest->getXY().x());
            break;

        case TranslateIcons::CROSS:

            //不进行变换保证确认
            start = getRandBlockInCross();
            dest = getRandBlockInCross();
            break;
        case TranslateIcons::ROTATE:
            break;
        case TranslateIcons::HEX:
            break;
        }

        //操作记录进sysRecord中
        bool success = selectAndSwitch(types[index],start,dest);
        if(success){
            sysRecord.append(GameRecord(types[index],start->getXY(),dest->getXY()),true);
        }
    }


    qDebug() << "系统打乱了方块 " << sysRecord.size() << "次。";
    for (int n = 0; n < sysRecord.size(); ++n) {
        sysRecord[n].info();
    }
}

//重置关卡
void Gamesys::resetLevel()
{
    //重置内容
    contents = "";
    curTransType = TranslateIcons::NONE;
    useStep = 0;
    levelscene->setStep(0);
    cancelSelect();
    tipPage->resetAnswer();


    //重置记录
    undoRecords.clear();
    playerRecords.clear();
    sysRecord.clear();
    levelscene->undoBtn->setOpacity(0.5);
    levelscene->doBtn->setOpacity(0.5);


    //回收textBlocks
    for (int n = 0; n < textBlocks.size(); ++n) {
        if(currentMode == CLASSIC){
            RectTextBlock* r = static_cast<RectTextBlock*>(textBlocks[n]);
            objPool::getinstance()->recycle(r);
            levelscene->removeItem(r);
        }else if(currentMode == HEX){
            HexTextBlock* h = static_cast<HexTextBlock*>(textBlocks[n]);
            objPool::getinstance()->recycle(h);
            levelscene->removeItem(h);
        }

    }
    textBlocks.clear();
    qDebug() << "reset success.";
}


//返回随机一个textBlock
RectTextBlock* Gamesys::getRandBlock()
{
    int rx = QRandomGenerator::global()->bounded(0,cols);
    int ry = QRandomGenerator::global()->bounded(0,rows);

    //qDebug() << "rx ry" << rx << ry;

    for (int n = 0; n < textBlocks.size(); ++n) {
        if(textBlocks[n]->getXY() == QPoint(rx,ry)){
            return static_cast<RectTextBlock*>(textBlocks[n]);
        }
    }
    qDebug() << "nullptr error";
    return nullptr;
}

//返回随机一个不位于边缘的textBlock
RectTextBlock* Gamesys::getRandBlockInCross()
{
    int rx = QRandomGenerator::global()->bounded(1,cols-1);
    int ry = QRandomGenerator::global()->bounded(1,rows-1);

    for (int n = 0; n < textBlocks.size(); ++n) {
        qDebug() << textBlocks[n]->getWord() << "be find at" << textBlocks[n]->getXY();
        if(textBlocks[n]->getXY() == QPoint(rx,ry)){
            return static_cast<RectTextBlock*>(textBlocks[n]);
        }
    }
    qDebug() << "nullptr error";
    return nullptr;
}

//算法：返回位于目标位置的textblock
RectTextBlock *Gamesys::findBlockAt(QPoint target)
{
    for (int n = 0; n < textBlocks.size(); ++n) {
        if(textBlocks[n]->getXY() == target){
            return static_cast<RectTextBlock*>(textBlocks[n]);
        }
    }
    qDebug() << "find block at nullptr错误";
    return nullptr;
}


//对接信号：sidebar的changetranstype
void Gamesys::changeTransType(TranslateIcons::Type type)
{
    curTransType = type;
    qDebug() << "curtranstype" << curTransType;
    cancelSelect();
}

//直接进行选择+变换，洗牌、undo/do使用。
bool Gamesys::selectAndSwitch(int type,TextBlock *start, TextBlock *dest)
{
    bool success = true;
    switch(type){

    case TranslateIcons::ROWS:
        rowSelect(start);
        success = rowSwitch(dest);
        break;
    case TranslateIcons::COLS:
        colSelect(start);
        success = colSwitch(dest);
        break;
    case TranslateIcons::CROSS:
        crossSelect(start);
        success = crossSwitch(dest);
        break;
    case TranslateIcons::ROTATE:
        break;
    case TranslateIcons::HEX:
        break;
    }

    cancelSelect();

    return success;
}




//对接信号：textblock的click(TextBlock*),根据hasSelects进行选择/变换
//变换成功时添加进playertrans记录表
void Gamesys::selectBlocks(TextBlock * target)
{
    switch(curTransType){

    case TranslateIcons::ROWS:
        rowSelect(target);
        break;
    case TranslateIcons::COLS:
        colSelect(target);
        break;
    case TranslateIcons::CROSS:
        crossSelect(target);
        break;
    case TranslateIcons::ROTATE:
        break;
    case TranslateIcons::HEX:
        break;
    }

    if(!selectedBlocks.isEmpty() || !crossTransMap.isEmpty()){
        hasSelects = true;
    }

    temp = target;
}

void Gamesys::switchBlocks(TextBlock * dest)
{

    bool success = true;
    switch(curTransType){

    case TranslateIcons::ROWS:
        success = rowSwitch(dest);
        break;
    case TranslateIcons::COLS:
        success = colSwitch(dest);
        break;
    case TranslateIcons::CROSS:
        success = crossSwitch(dest);
        break;
    case TranslateIcons::ROTATE:
        break;
    case TranslateIcons::HEX:
        break;
    }

    //成功变换时，播放音效、添加进记录，重置do功能, useStep + 1.
    if(success){
        MusicPlayer::getMPlayer()->playBlockSound(true);
        addRecord(GameRecord(curTransType,temp->getXY(),dest->getXY()));
        cancelSelect();

        undoRecords.clear();
        levelscene->doBtn->setOpacity(0.5);

        ++useStep;
        levelscene->setStep(useStep);

        checkIfComplete();

    }else{
        MusicPlayer::getMPlayer()->playBlockSound(false);
    }


}

//选择细项
void Gamesys::rowSelect(TextBlock *target)
{
    for (TextBlock* t : textBlocks) {
        if(t->getXY().y() == target->getXY().y()){
            t->showBorder();
            selectedBlocks.push_back(t);
        }
    }
}
void Gamesys::colSelect(TextBlock *target)
{
    for (TextBlock* t : textBlocks) {
        if(t->getXY().x() == target->getXY().x()){
            t->showBorder();
            selectedBlocks.push_back(t);
        }
    }
}
void Gamesys::crossSelect(TextBlock *target)
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
        t->showBorder();
    }
}


//变换细项，返回值表示是否变换成功。
bool Gamesys::rowSwitch(TextBlock *dest)
{

    QList<TextBlock*> blocksForSwitch;
    for (TextBlock* t : textBlocks){
        if(t->getXY().y() == dest->getXY().y()){
            blocksForSwitch.push_back(t);
        }
    }

    for (int x = 0; x < cols; ++x) {
        if(blocksForSwitch[x]->getXY().y() == selectedBlocks[x]->getXY().y()){
            return false;
        }
        TextBlock::switchWord(blocksForSwitch[x],selectedBlocks[x]);
    }
    return true;
}
bool Gamesys::colSwitch(TextBlock *dest)
{
    QList<TextBlock*> blocksForSwitch;
    for (TextBlock* t : textBlocks){
        if(t->getXY().x() == dest->getXY().x()){
            blocksForSwitch.push_back(t);
        }
    }

    for (int y = 0; y < rows; ++y) {
        if(blocksForSwitch[y]->getXY().x() == selectedBlocks[y]->getXY().x()){
            return false;
        }
        TextBlock::switchWord(blocksForSwitch[y],selectedBlocks[y]);
    }
    return true;;
}
bool Gamesys::crossSwitch(TextBlock *dest)
{
    {
        //dest 不能为边缘地区
        if(dest->getXY().x() == 0 || dest->getXY().x() == cols-1
            || dest->getXY().y() == 0 || dest->getXY().y() == rows-1)
        {
            qDebug() << "不能选择边缘";
            return false;
        }

        //不能有重叠区块
        if((dest->getXY() - crossTransMap["CENTER"]->getXY()).manhattanLength() <= 2){
            qDebug() << "不能有重叠区域";
            return false;
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

        RectTextBlock::switchWord(dest,crossTransMap["CENTER"]);
        return true;
    }
}


//取消选择
void Gamesys::cancelSelect()
{
    temp = nullptr;
    hasSelects = false;

    for (TextBlock* t : selectedBlocks) {
        t->hideBorder();
    }
    selectedBlocks.clear();

    for (TextBlock* t : crossTransMap) {
        t->hideBorder();
    }
    crossTransMap.clear();
}


//do undo
void Gamesys::DoTrans()
{
    //先检查是否可do
    if(undoRecords.isEmpty()){
        return;
    }

    if(currentMode == CLASSIC){

        GameRecord record = undoRecords.pop();
        TextBlock *start,*dest;

        for (TextBlock* t : textBlocks) {
            if(t->getXY() == record.startXY){
                start = t;
            }
            if(t->getXY() == record.toXY){
                dest = t;
            }
        }

        //undoRecords储存的是player变换的起点/终点，不需要反向操作。
        selectAndSwitch(record.type,start,dest);

        //加回到playerRecord
        addRecord(record);
    }



    //useStep还原
    ++useStep;
    levelscene->setStep(useStep);

    //检查do是否可用
    if(undoRecords.isEmpty()){
        levelscene->doBtn->setOpacity(0.5);
    }
}

void Gamesys::UndoTrans()
{
    //先检查是否可undo
    if(playerRecords.isEmpty()){
        return;
    }

    if(currentMode == CLASSIC){
        GameRecord record = playerRecords.pop();
        TextBlock *oldStart,*oldDest;

        for (TextBlock* t : textBlocks) {
            if(t->getXY() == record.startXY){
                oldStart = t;
            }
            if(t->getXY() == record.toXY){
                oldDest = t;
            }
        }

        //反向操作即可
        selectAndSwitch(record.type,oldDest,oldStart);

        undoRecords.append(GameRecord(record.type,record.startXY,record.toXY),false);
    }



    --useStep;
    levelscene->setStep(useStep);

    //启用doBtn
    levelscene->doBtn->setOpacity(1);

    //检查undo是否可用
    if(playerRecords.isEmpty()){
        levelscene->undoBtn->setOpacity(0.5);
    }
}




//添加操作记录到playerrecord中，启用undo按钮
void Gamesys::addRecord(GameRecord record)
{
    playerRecords.append(record,false);
    record.info();
    levelscene->undoBtn->setOpacity(1);
}

void Gamesys::checkIfComplete()
{
    QString t("");

    if(currentMode == CLASSIC){
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                t.append(textBlocks[x + y*cols]->getWord());
            }
        }
    }


    if(t == contents){
        completeGame();
    }
}


void Gamesys::completeGame()
{
    //得分 = 100 - 系统变换次数与使用步数之差。（暂定）
    completePage->showUseStep(useStep);
    tempview->setScene(completePage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);
}










