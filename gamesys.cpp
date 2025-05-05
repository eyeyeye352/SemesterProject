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

    //other scene
    settingPage = new SettingPage(this);
    tipPage = new TipPage(this);
    completePage = new CompletePage(this);


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
        connect(startscene->levels[n],&LevelSelectBlock::selected,this,&Gamesys::loadGameAnime);
    }

    //connections(levelScene)
    connect(levelscene->settingBtn,&GameBtn::clicked,this,&Gamesys::openSetting);
    connect(levelscene->rankBtn,&GameBtn::clicked,this,&Gamesys::checkRank);
    connect(levelscene->saveBtn,&GameBtn::clicked,this,&Gamesys::openSavePage);
    connect(levelscene->tipBtn,&GameBtn::clicked,this,&Gamesys::openTips);
    connect(levelscene->undoBtn,&GameBtn::clicked,this,&Gamesys::UndoTrans);
    connect(levelscene->doBtn,&GameBtn::clicked,this,&Gamesys::DoTrans);
    connect(levelscene->sideBar,&MySideBar::SelectTransType,this,&Gamesys::changeTransType);
    connect(levelscene,&LevelScene::clickBg,this,&Gamesys::cancelSelect);

    //connection(settingpage)
    connect(settingPage,&SettingPage::backHome,this,&Gamesys::backHome);
    connect(settingPage,&SettingPage::closeSetting,this,&Gamesys::closeSetting);

    //connection(tipPage)
    connect(tipPage,&TipPage::closeTip,this,&Gamesys::closeTips);

    //connection(completePage)
    connect(completePage,&CompletePage::backHome,this,&Gamesys::backHome);



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

    qDebug() << "choose mode:" << currentMode;
}


void Gamesys::backModeSelection()
{
    Animation::backModeSelection(startscene);
}


//子功能页面 settingPage、tipPage、rankPage、savePage
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
void Gamesys::openTips()
{
    tempview->setScene(tipPage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);
}
void Gamesys::closeTips()
{
    QPropertyAnimation* anime = Animation::TempPageout(view,tempview);
    anime->start(QAbstractAnimation::DeleteWhenStopped);

    connect(anime,&QPropertyAnimation::finished,[this]{
        tempview->setScene(nullptr);
    });
}

//设置页面返回首页
void Gamesys::backHome()
{

    if(view->scene() == startscene){
        closeSetting();
    }

    //关闭tempview动画结束后，返回首页
    else if(view->scene() == levelscene){

        for (int n = 0 ; n < textBlocks.size() ; ++n) {
            objPool::getinstance()->recycle(textBlocks[n]);
            levelscene->removeItem(textBlocks[n]);
        }

        QPropertyAnimation* anime = Animation::TempPageout(view,tempview);

        //设置页面退出动画
        connect(anime,&QPropertyAnimation::finished,[this]{
            Animation::changeScene(levelscene,startscene,view,1500)->start(QAbstractAnimation::DeleteWhenStopped);
            MusicPlayer::getMPlayer()->changeBgm(QUrl("qrc:/bgm/src/bgm/startSceneBGM.mp3"));
            tempview->setScene(nullptr);
        });

        anime->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void Gamesys::openSavePage()
{

}
void Gamesys::closeSavePage()
{

}




void Gamesys::checkRank()
{

}

void Gamesys::goCreateMode()
{

}

//切换音乐和scene,然后在loadingscene动画中加载游戏
void Gamesys::loadGameAnime(int levelNum)
{

    MusicPlayer::getMPlayer()->changeBgm(QUrl("qrc:/bgm/src/bgm/ingameBgm.mp3"));
    QSequentialAnimationGroup* anime = Animation::changeScene(startscene,levelscene,view,3000);
    connect(anime->animationAt(1),&QPropertyAnimation::finished,[this,levelNum]{
        //动画暂停时加载文件
        loadGame(levelNum);
    });
    anime->start(QAbstractAnimation::DeleteWhenStopped);
}

//加载文件，渲染textblocks
void Gamesys::loadGame(int levelNum)
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
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);

    //加载前先重置各种成员
    resetLevel();

    //处理字符串
    QString whole = file.readAll();
    QStringList strList = whole.split("\r\n");

    for (int line = 0; line < strList.size(); ++line) {

        //title部分
        if(strList[line].startsWith("title=")){
            levelscene->setTitle(strList[line].remove("title="));
        }

        //难度（行列数）部分
        else if(strList[line].startsWith("difficulty=")){

            //regexp确保格式匹配"数字,数字"
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
            if(t == "classic") currentMode = CLASSIC;
            else if(t == "hex") currentMode = HEX;
            levelscene->setGameMode(currentMode);
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
    qDebug() << "rows,cols:" << rows << ',' << cols;
    qDebug() << "content" << contents;

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {

            //每个textBlock储存其位置[左上角：（0,0）~ 右下角 (cols,rows)]与一个汉字
            //目标汉字位于 contents的 y*每row字数 (即y*cols) + x
            QString word = contents[y*cols + x];
            TextBlock * t = objPool::getinstance()->getTextBlock();
            t->setWord(word);
            t->setxy({x,y});
            textBlocks.append(t);

            //每个方块链接信号
            disconnect(t, &TextBlock::clicked, nullptr, nullptr);
            connect(t,&TextBlock::clicked,this,[this](TextBlock* t){
                hasSelects ? switchBlocks(t) : selectBlocks(t);
            },Qt::UniqueConnection);

            //方块交给levelscene渲染
            levelscene->addTextBlock(t,rows,cols);
        }
    }

    qDebug() << "loading finish.";

    file.close();
    tipPage->setAnswer(contents,cols);

    //洗牌
    shuffleLevel(levelNum);

}



//载入关卡时的洗牌算法,成功变换时添加进systrans记录表
void Gamesys::shuffleLevel(int levelNum)
{
    //变换次数
    int transtimes = levelNum*5;


    //变换模式列表
    QList<TranslateIcons::Type> types =
        {
            TranslateIcons::ROWS,
            TranslateIcons::COLS,
            TranslateIcons::CROSS,
            TranslateIcons::HEXLINE,
            TranslateIcons::HEX
        };



    //变换直到sysrecord有效记录次数达到指定次数
    while(sysRecord.size() < transtimes){

        int num;
        //随机选一种变换方式（classic关卡为1，3时不进行十字变换）
        if(currentMode == CLASSIC && (levelNum == 1 || levelNum == 3)){
            num = QRandomGenerator::global()->bounded(0,2);
        }
        else if(currentMode == CLASSIC){
            num = QRandomGenerator::global()->bounded(0,3);
        }

        //hex模式
        else if(currentMode == HEX){
            num = QRandomGenerator::global()->bounded(3,5);
            //先退出，还未开发
            break;
        }


        //随机选两个textblock
        TextBlock* start , *dest;

        switch(types[num]){
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
        case TranslateIcons::HEXLINE:
            break;
        case TranslateIcons::HEX:
            break;
        }

        //操作记录进sysRecord中
        bool success = selectAndSwitch(types[num],start,dest);
        if(success){
            sysRecord.append(GameRecord(types[num],start->getXY(),dest->getXY()),true);
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
    tipPage->setAnswer("",0);

    //重置记录
    undoRecords.clear();
    playerRecords.clear();
    sysRecord.clear();
    levelscene->undoBtn->setOpacity(0.5);
    levelscene->doBtn->setOpacity(0.5);


    //回收textBlocks
    for (int n = 0; n < textBlocks.size(); ++n) {
        objPool::getinstance()->recycle(textBlocks[n]);
        levelscene->removeItem(textBlocks[n]);
    }
    textBlocks.clear();

    qDebug() << "reset success.";
}


//返回随机一个textBlock
TextBlock* Gamesys::getRandBlock()
{
    int rx = QRandomGenerator::global()->bounded(0,cols);
    int ry = QRandomGenerator::global()->bounded(0,rows);

    for (TextBlock* t : textBlocks) {
        if(t->getXY() == QPoint(rx,ry)){
            return t;
        }
    }
    return nullptr;
}

//返回随机一个不位于边缘的textBlock
TextBlock* Gamesys::getRandBlockInCross()
{
    int rx = QRandomGenerator::global()->bounded(1,cols-1);
    int ry = QRandomGenerator::global()->bounded(1,rows-1);

    for (TextBlock* t : textBlocks) {
        if(t->getXY() == QPoint(rx,ry)){
            return t;
        }
    }
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
bool Gamesys::selectAndSwitch(TranslateIcons::Type type,TextBlock *start, TextBlock *dest)
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
    case TranslateIcons::HEXLINE:
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
    case TranslateIcons::HEXLINE:
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
    case TranslateIcons::HEXLINE:
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
            t->showGoldRect();
            selectedBlocks.push_back(t);
        }
    }
}
void Gamesys::colSelect(TextBlock *target)
{
    for (TextBlock* t : textBlocks) {
        if(t->getXY().x() == target->getXY().x()){
            t->showGoldRect();
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
        t->showGoldRect();
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

        TextBlock::switchWord(dest,crossTransMap["CENTER"]);
        return true;
    }
}


//取消选择
void Gamesys::cancelSelect()
{
    temp = nullptr;
    hasSelects = false;

    for (TextBlock* t : selectedBlocks) {
        t->hideGoldRect();
    }
    selectedBlocks.clear();

    for (TextBlock* t : crossTransMap) {
        t->hideGoldRect();
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

    --useStep;
    levelscene->setStep(useStep);

    //启用doBtn
    undoRecords.append(GameRecord(record.type,record.startXY,record.toXY),false);
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
    QString temp("");
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            temp.append(textBlocks[x + y*cols]->Word());
        }
    }

    if(temp == contents){
        completeGame();
    }
}


void Gamesys::completeGame()
{
    //得分 = 100 - 系统变换次数与使用步数之差。（暂定）
    completePage->setcontents(useStep,100 - abs(useStep - sysRecord.size()));
    tempview->setScene(completePage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);
}












