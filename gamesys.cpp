#include "gamesys.h"
#include "animation.h"
#include "myalgorithms.h"


Gamesys::Gamesys(QWidget *parent)
    : QWidget(parent)
{
    //bgm
    MusicPlayer::getMPlayer()->startBGM();

    //窗口设置
    setFixedSize(Settings::screenWidth,Settings::screenHeight);
    setWindowIcon(QIcon(Settings::gameIconPath));

    //scene and view
    startscene = new StartScene(this);
    levelscene = new LevelScene(this);
    createscene = new CreateScene(this);

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
    bgmoveTimer1 = new QTimer(this);
    bgmoveTimer1->start(Settings::backgroundUpdateInterval);
    connect(bgmoveTimer1,&QTimer::timeout,startscene,&StartScene::moveBG);



    //startScene connections
    connect(startscene->classicBtn,&ClassicBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->hexBtn,&HexBtn::clicked,this,&Gamesys::goLevelSelection);
    connect(startscene->settingBtn,&GameBtn::clicked,this,&Gamesys::openSettingPage);
    connect(startscene->createModeBtn,&GameBtn::clicked,this,&Gamesys::goCreateScene);
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

    //connections(createscene)
    connect(createscene->classicBtn,&ClassicBtn::clicked,this,&Gamesys::goCreate);
    connect(createscene->hexBtn,&HexBtn::clicked,this,&Gamesys::goCreate);
    connect(createscene->settingBtn,&GameBtn::clicked,this,&Gamesys::openSettingPage);
    connect(createscene->rankBtn,&GameBtn::clicked,this,&Gamesys::openCreateRankPage);
    connect(createscene->backBtn,&GameBtn::clicked,this,&Gamesys::goHome);

    //connection(otherpage)
    connect(settingPage,&SettingPage::goHome,this,&Gamesys::goHome);
    connect(settingPage,&TempPage::closePage,this,&Gamesys::closeTempPage);

    connect(tipPage,&TempPage::closePage,this,&Gamesys::closeTempPage);

    connect(completePage,&CompletePage::goHome,this,&Gamesys::goHome);

    connect(savePage,&TempPage::closePage,this,&Gamesys::closeTempPage);
    connect(savePage,&SavePage::slotSelected,this,&Gamesys::processSlots);

    connect(rankPage,&TempPage::closePage,this,&Gamesys::closeTempPage);
    connect(rankPage,&RankPage::orderByStep,this,&Gamesys::orderByStep);
    connect(rankPage,&RankPage::orderByTime,this,&Gamesys::orderByTime);

    initSLSlot();
    objPool::getinstance()->init();

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

    qDebug() << "select mode:" << currentMode;
}


void Gamesys::backModeSelection()
{
    Animation::backModeSelection(startscene);
}

void Gamesys::goCreateScene()
{
    //更换bgm
    MusicPlayer::getMPlayer()->changeBgm(Settings::bgmList["InGame_classicMode"]);
    QSequentialAnimationGroup* anime = Animation::changeScene(startscene,createscene,view,3000);
    anime->start(QAbstractAnimation::DeleteWhenStopped);

    Animation::bgMoveLoopAnime(createscene->bg1);

}

void Gamesys::goCreate(Mode m)
{
    qDebug() << "go create mode :" << m;
}

void Gamesys::openCreateRankPage()
{
    qDebug() << "open rankpage in create mode.";
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
    tempview->setScene(rankPage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);
}

//设置页面返回首页
void Gamesys::goHome()
{

    if(view->scene() == startscene){
        closeTempPage();
    }

    //关闭tempview动画结束后，返回首页

    //developing:(询问用户是否保存？)
    else{

        if(view->scene() == levelscene){
            resetLevel();
        }

        QPropertyAnimation* anime = Animation::TempPageout(view,tempview);

        //设置页面退出动画
        connect(anime,&QPropertyAnimation::finished,[this]{
            Animation::changeScene(static_cast<Scene*>(view->scene()),startscene,view,1500)->start(QAbstractAnimation::DeleteWhenStopped);
            MusicPlayer::getMPlayer()->changeBgm(Settings::bgmList["pastel_subminal"]);
            tempview->setScene(nullptr);
        });

        anime->start(QAbstractAnimation::DeleteWhenStopped);
    }
}


//关闭临时弹窗
void Gamesys::closeTempPage()
{
    QPropertyAnimation* anime = Animation::TempPageout(view,tempview);
    anime->start(QAbstractAnimation::DeleteWhenStopped);

    connect(anime,&QPropertyAnimation::finished,[this]{
        tempview->setScene(nullptr);
    });
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

    //加载动画结束，开始计时
    connect(anime,&QSequentialAnimationGroup::finished,[this]{
        startTiming = QTime::currentTime();
    });
    anime->start(QAbstractAnimation::DeleteWhenStopped);
}


//加载文件
void Gamesys::loadGame(int levelNum,bool shuffled)
{

    //根据模式选择关卡文件
    QString filepath;
    if(currentMode == CLASSIC){
        filepath = QString(":/Levels/src/levelsDoc/ClassicL%1.txt").arg(levelNum);
    }
    else if(currentMode == HEX){
        filepath = QString(":/Levels/src/levelsDoc/HexL%1.txt").arg(levelNum);
    }

    //加载前先重置关卡
    resetLevel();

    //设置rankpage
    loadRankList(levelNum);

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

            //内容共有几行
            int contentLines;

            if(currentMode == CLASSIC){
                contentLines = rows;
            }else if(currentMode == HEX){
                contentLines = 2*radius - 1;
            }

            //注意n不要越界。
            for (int n = line+1; n <= line+contentLines && n < strList.size(); ++n) {
                contents.append(strList[n]);
            }
            break;
        }
    }

    //debug信息
    qDebug() << "content read" << contents;

    loadTextBlocks();

    qDebug() << "loading finish.";


    //设置提示页面的答案
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
            //防止格式错误时崩溃
            rows = 0;
            cols = 0;
        }

        qDebug() << "rows,cols be read:" << rows << "," << cols;
    }
    else if(currentMode == HEX){
        radius = d_line.remove("difficulty=").toInt();
        qDebug() << "radius be read:" << radius;
    }

}

void Gamesys::loadTextBlocks()
{

    //临时指针，指向中心hex
    HexTextBlock* center;

    if(currentMode == CLASSIC){
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {

                //每个recttextBlock储存其位置[左上角：（0,0）~ 右下角 (cols,rows)]与一个汉字
                //目标汉字位于 contents的 y*每row字数 (即y*cols) + x
                QString word = contents[y*cols + x];
                RectTextBlock * t = objPool::getinstance()->getRectTextBlock();
                t->setWord(word);
                t->setxy({x,y});
                textBlocks.append(t);

                //每个方块链接信号(注意不要重复连接)
                disconnect(t, &RectTextBlock::clicked, nullptr, nullptr);
                connect(t,&RectTextBlock::clicked,this,[this](RectTextBlock* t){
                    hasSelects ? switchBlocks(t) : selectBlocks(t);
                });

                //方块交给levelscene渲染
                levelscene->addTextBlock(t,rows,cols);
            }
        }
    }

    else if(currentMode == HEX){

        //全部共有 3n(n-1)+1个小六边形。
        QList<QPoint> hexCoordis = MyAlgorithms::makeHexCoordi(radius);

        for (int N = 0; N < 3*radius*(radius-1)+1; ++N) {

            HexTextBlock * h = objPool::getinstance()->getHexTextBlock();
            h->setWord(contents[N]);
            h->setHpos(hexCoordis[N]);

            //每个方块链接信号
            disconnect(h, &HexTextBlock::clicked, nullptr, nullptr);
            connect(h,&HexTextBlock::clicked,this,[this](HexTextBlock* h){
                    hasSelects ? switchBlocks(h) : selectBlocks(h);
            });

            textBlocks.append(h);

            //定位中心
            if(h->getXY() == QPoint(0,0)){
                center = h;
            }
        }

        //设置hex的邻居关系
        for (int n = 0; n < textBlocks.size(); ++n) {
            for (int nn = 0; nn < textBlocks.size(); ++nn) {
                QPoint dis = textBlocks[nn]->getXY() - textBlocks[n]->getXY();
                //符合以下条件，即为邻居
                if(dis == QPoint(1,0) || dis == QPoint(1,1)
                    || dis == QPoint(0,1) || dis == QPoint(-1,0)
                    || dis == QPoint(-1,-1) || dis == QPoint(0,-1))
                {
                    static_cast<HexTextBlock*>(textBlocks[n])->addNeightBor(textBlocks[nn]);
                }
            }
        }

        //设置好邻居关系后，渲染其位置
        levelscene->addCenterHexBlock(center,Settings::hexBlockSize);
    }


    //debug信息
    for (int n = 0; n < textBlocks.size(); ++n) {
        TextBlock* t = textBlocks[n];
        qDebug() << t->getWord() << " at:" << t->getXY();
    }
}

//初始加载本地文件显示在slot上。
void Gamesys::initSLSlot()
{

    // 获取用户文档目录，并创建一个名为 /gameSavings 的子目录
    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/gameSavings";
    QDir().mkpath(saveDir); // 创建保存目录，如果不存在

    for (int n = 0; n < savePage->getSlots().size(); ++n) {
        QString filepath = QDir(saveDir).filePath("s%1.txt");
        qDebug() << "init slots by:" << filepath.arg(QString::number(n));
        //存档信息记录在slot中
        savePage->getSlots()[n]->init(filepath.arg(QString::number(n)));
    }

}

void Gamesys::processSlots(int slotNum, int state)
{
    if(state == SavePage::NONE){
        //show slot info and do nothing，输出调试信息
        savePage->getSlots()[slotNum]->info();
    }
    else if(state == SavePage::SAVE){
        //developing（hexmode）
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

    //防止bug：存档切换游戏模式时回收错误类型的textblock
    resetLevel();


    //获取各种属性
    curLevelNum = S->getLevelNum();
    currentMode = S->getGameMode();


    //先关闭savePage，切换场景(关卡)、不洗牌。
    closeTempPage();

    if(view->scene() == startscene){
        MusicPlayer::getMPlayer()->changeBgm(Settings::bgmList["InGame_classicMode"]);
    }

    QSequentialAnimationGroup* anime = Animation::changeScene(static_cast<Scene*>(view->scene()),levelscene,view,3500);
    connect(anime->animationAt(1),&QPropertyAnimation::finished,[=]{


        //动画暂停时加载文件 (注意加载时会Reset一次)
        loadGame(curLevelNum,false);


        //载入存档记录
        sysRecord = S->getSysRecords();
        playerRecords = S->getPlayerRecords();
        recordTiming = S->getTime_spending();

        for (int n = 0; n < sysRecord.size(); ++n) {
            GameRecord r = sysRecord[n];
            selectAndSwitch(r.type,findBlockAt(r.startXY),findBlockAt(r.toXY));
        }

        //启用undo
        if(playerRecords.size() > 0){
            levelscene->undoBtn->setOpacity(1);
        }

        for (int n = 0; n < playerRecords.size(); ++n) {
            GameRecord r = playerRecords[n];
            selectAndSwitch(r.type,findBlockAt(r.startXY),findBlockAt(r.toXY));
        }

        //设置步数
        useStep = playerRecords.size();
        levelscene->setStep(useStep);
    });

    //开始计时
    connect(anime,&QSequentialAnimationGroup::finished,[=]{
        //转成秒数
        startTiming = QTime::currentTime().addMSecs(-recordTiming.msecsSinceStartOfDay());
        qDebug() << "starttiming:" << startTiming;
        qDebug() << "recordtiming:" << recordTiming;
    });
    anime->start(QAbstractAnimation::DeleteWhenStopped);
}

void Gamesys::loadRankList(int levelNum)
{
    QString rankDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/gameRank";
    QDir().mkpath(rankDir); // 创建保存目录，如果不存在
    QString filepath;

    if(currentMode == CLASSIC){
        filepath = rankDir + QString("/classic%1.txt").arg(levelNum);
    }else if(currentMode == HEX){
        filepath = rankDir + QString("/hex%1.txt").arg(levelNum);
    }

    QString content = MyAlgorithms::getContentInFile(filepath);
    qDebug() << "rank content:" << content;
    QString fixedContent = content.replace("\r\n","\n");
    QStringList linelist = fixedContent.split('\n');
    for (int n = 0; n < linelist.size(); ++n) {
        //跳过空行
        if(linelist[n] != ""){
            RankRecord r(linelist[n]);
            rankRecords.append(r);
        }
    }

    //根据通关用时排序并显示
    orderByTime();

}

void Gamesys::orderByTime()
{
    std::sort(rankRecords.begin(),rankRecords.end(),[](const RankRecord& r1,const RankRecord& r2){
        if(r1.time_spending != r2.time_spending){
            return r1.time_spending < r2.time_spending;
        }
        else{
            return r1.step_using < r2.step_using;
        }
    });
    rankPage->showContents(rankRecords);
}

void Gamesys::orderByStep()
{
    std::sort(rankRecords.begin(),rankRecords.end(),[](const RankRecord& r1,const RankRecord& r2){
        if(r1.step_using != r2.step_using){
            return r1.step_using < r2.step_using;
        }
        else{
            return r1.time_spending < r2.time_spending;
        }
    });
    rankPage->showContents(rankRecords);
}

void Gamesys::addRankRecord(RankRecord r)
{
    while(rankRecords.size() > 99){
        rankRecords.pop_back();
    }
    rankRecords.append(r);
    QString rankDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/gameRank";
    QDir().mkpath(rankDir); // 创建保存目录，如果不存在
    QString filepath;

    if(currentMode == CLASSIC){
        filepath = rankDir + QString("/classic%1.txt").arg(curLevelNum);
    }else if(currentMode == HEX){
        filepath = rankDir + QString("/hex%1.txt").arg(curLevelNum);
    }

    //更新本地文件
    QFile file(filepath);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);
    for (int n = 0; n < rankRecords.size(); ++n) {

        if(n != rankRecords.size()-1){
            stream << rankRecords[n].toString() << '\n';
        }else{
            stream << rankRecords[n].toString();
        }

    }
    file.close();
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

    //time——spending
    QString timeSpendingStr = MyAlgorithms::timeInterval(startTiming,QTime::currentTime());

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
    QString finaltext = origtext.arg(savetime).arg(mode).arg(levelNumstr).arg(timeSpendingStr).arg(rNum);
    for (int n = 0; n < gamerecords.size(); ++n) {
        finaltext.append(gamerecords[n]);
    }
    out << finaltext;

    file.close();
}



//载入关卡时的洗牌算法,成功变换时添加进systrans记录表
void Gamesys::shuffleLevel()
{
    //变换次数随关卡数字提升
    int transtimes;
    if(currentMode == CLASSIC){
        transtimes = curLevelNum*5;
    }else if(currentMode == HEX){
        transtimes = curLevelNum*3;
    }

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
        //hex模式（同样，1，3关不进行hex变换）
        else if(currentMode == HEX && (curLevelNum == 1 || curLevelNum == 3)){
            index = 3;
        }
        else if(currentMode == HEX){
            index = QRandomGenerator::global()->bounded(3,5);
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

            start = dest = getRandBlockNotAtBorder();
            break;
        case TranslateIcons::HEX:

            //不进行变换确认
            start = getRandBlockNotAtBorder();
            dest = getRandBlockNotAtBorder();
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
    //rows cols radius isselected都会在别处重置。
    //重置内容
    contents = "";
    curTransType = TranslateIcons::NONE;
    useStep = 0;
    levelscene->setStep(0);
    cancelSelect();
    tipPage->resetAnswer();
    recordTiming = QTime(0,0,0,0);


    //重置记录
    undoRecords.clear();
    playerRecords.clear();
    sysRecord.clear();
    rankRecords.clear();
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

//返回一个不位于边缘的hextextblock
HexTextBlock *Gamesys::getRandBlockNotAtBorder()
{
    refind:
    int n = QRandomGenerator::global()->bounded(0,textBlocks.size());
    if(static_cast<HexTextBlock*>(textBlocks[n])->atBorder()){
        //不能在边缘
        goto refind;
    }

    return static_cast<HexTextBlock*>(textBlocks[n]);
}

//算法：返回位于目标位置的textblock
TextBlock *Gamesys::findBlockAt(QPoint target)
{
    for (int n = 0; n < textBlocks.size(); ++n) {
        if(textBlocks[n]->getXY() == target){
            return textBlocks[n];
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

//直接进行选择+变换，洗牌、undo（undo rotate时另采其他方案）/do使用。
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
        success = AntiRotate(static_cast<HexTextBlock*>(start));
        break;
    case TranslateIcons::HEX:
        hexSelect(static_cast<HexTextBlock*>(start));
        success = hexSwitch(static_cast<HexTextBlock*>(dest));
        break;
    }

    cancelSelect();
    return success;
}




//对接信号：textblock的click(TextBlock*),根据hasSelects进行选择/变换
//变换成功时添加进playertrans记录表
void Gamesys::selectBlocks(TextBlock * target)
{

    temp = target;

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
        //旋转无选取概念，直接转
        switchBlocks(target);
        break;
    case TranslateIcons::HEX:
        hexSelect(static_cast<HexTextBlock*>(target));
        break;
    }

    if(!selectedBlocks.isEmpty() || !selectedBlockMap.isEmpty()){
        hasSelects = true;
    }


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
        success = AntiRotate(static_cast<HexTextBlock*>(dest));
        break;
    case TranslateIcons::HEX:
        success = hexSwitch(static_cast<HexTextBlock*>(dest));
        break;
    }

    //成功变换时，播放音效、添加进记录，重置do功能, useStep + 1.
    if(success){
        MusicPlayer::getMPlayer()->playBlockSound(true);

        addRecord(GameRecord(curTransType,temp->getXY(),dest->getXY()));
        cancelSelect();

        undoRecords.clear();
        levelscene->doBtn->setOpacity(0.5);

        levelscene->setStep(++useStep);

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
            selectedBlockMap.insert("UP",t);
        }
        //t == down
        else if(t->getXY() - target->getXY() == QPoint(0,1)){
            selectedBlockMap.insert("DOWN",t);
        }
        //t == left
        else if(t->getXY() - target->getXY() == QPoint(-1,0)){
            selectedBlockMap.insert("LEFT",t);
        }
        //t == right
        else if(t->getXY() - target->getXY() == QPoint(1,0)){
            selectedBlockMap.insert("RIGHT",t);
        }
    }
    selectedBlockMap.insert("CENTER",target);

    //加金色框框
    for (TextBlock* t : selectedBlockMap) {
        t->showBorder();
    }
}
void Gamesys::hexSelect(HexTextBlock *h)
{
    if(h->atBorder()){
        qDebug() << "无效选取";
        return;
    }

    QList<HexTextBlock*>& nbs = h->getNeightBors();
    for (int n = 0; n < nbs.size(); ++n) {
        //right
        if(nbs[n]->getXY() - h->getXY() == QPoint(1,0)){
            selectedBlockMap["right"] = nbs[n];
        }
        //left
        else if(nbs[n]->getXY() - h->getXY() == QPoint(-1,0)){
            selectedBlockMap["left"] = nbs[n];
        }
        //right up
        else if(nbs[n]->getXY() - h->getXY() == QPoint(1,1)){
            selectedBlockMap["rightUp"] = nbs[n];
        }
        //right down
        else if(nbs[n]->getXY() - h->getXY() == QPoint(0,-1)){
            selectedBlockMap["rightDown"] = nbs[n];
        }
        //left up
        else if(nbs[n]->getXY() - h->getXY() == QPoint(0,1)){
            selectedBlockMap["leftUp"] = nbs[n];
        }
        //leftDown
        else if(nbs[n]->getXY() - h->getXY() == QPoint(-1,-1)){
            selectedBlockMap["leftDown"] = nbs[n];
        }
        nbs[n]->showBorder();
    }

    temp = selectedBlockMap["center"] = h;
    h->showBorder();
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
    return true;
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
        if((dest->getXY() - selectedBlockMap["CENTER"]->getXY()).manhattanLength() <= 2){
            qDebug() << "不能有重叠区域";
            return false;
        }


        for (TextBlock* t : textBlocks){

            //t == dest-up
            if(t->getXY() - dest->getXY() == QPoint(0,-1)){
                TextBlock::switchWord(t,selectedBlockMap["UP"]);
            }
            //t == dest-down
            else if(t->getXY() - dest->getXY() == QPoint(0,1)){
                TextBlock::switchWord(t,selectedBlockMap["DOWN"]);
            }
            //t == dest-left
            else if(t->getXY() - dest->getXY() == QPoint(-1,0)){
                TextBlock::switchWord(t,selectedBlockMap["LEFT"]);
            }
            //t == dest-right
            else if(t->getXY() - dest->getXY() == QPoint(1,0)){
                TextBlock::switchWord(t,selectedBlockMap["RIGHT"]);
            }
        }

        RectTextBlock::switchWord(dest,selectedBlockMap["CENTER"]);
        return true;
    }
}
bool Gamesys::hexSwitch(HexTextBlock *h)
{
    if(h->atBorder()){
        return false;
    }
    //不能是邻居的邻居
    for (HexTextBlock* nb1 : static_cast<HexTextBlock*>(temp)->getNeightBors()) {
        for (HexTextBlock* nb2 : h->getNeightBors()) {
            if(nb1 == nb2){
                qDebug() << "不能有重复区域";
                return false;
            }
        }
    }

    QList<HexTextBlock*>& nbs = h->getNeightBors();


    for (int n = 0; n < nbs.size(); ++n) {
        //right
        if(nbs[n]->getXY() - h->getXY() == QPoint(1,0)){
            TextBlock::switchWord(nbs[n],selectedBlockMap["right"]);
        }
        //left
        else if(nbs[n]->getXY() - h->getXY() == QPoint(-1,0)){
            TextBlock::switchWord(nbs[n],selectedBlockMap["left"]);
        }
        //right up
        else if(nbs[n]->getXY() - h->getXY() == QPoint(1,1)){
            TextBlock::switchWord(nbs[n],selectedBlockMap["rightUp"]);
        }
        //right down
        else if(nbs[n]->getXY() - h->getXY() == QPoint(0,-1)){
            TextBlock::switchWord(nbs[n],selectedBlockMap["rightDown"]);
        }
        //left up
        else if(nbs[n]->getXY() - h->getXY() == QPoint(0,1)){
            TextBlock::switchWord(nbs[n],selectedBlockMap["leftUp"]);
        }
        //leftDown
        else if(nbs[n]->getXY() - h->getXY() == QPoint(-1,-1)){
            TextBlock::switchWord(nbs[n],selectedBlockMap["leftDown"]);
        }
    }

    TextBlock::switchWord(h,selectedBlockMap["center"]);

    for (TextBlock* hs : selectedBlockMap) {
        hs->hideBorder();
    }
    return true;
}

//顺时针旋转仅unditrans使用，必定成功，无需返回值
void Gamesys::NRotate(HexTextBlock *h)
{
    QList<HexTextBlock*>& nbs = h->getNeightBors();
    QMap<QString,HexTextBlock*> rotMap;

    for (int n = 0; n < nbs.size(); ++n) {
        //right
        if(nbs[n]->getXY() - h->getXY() == QPoint(1,0)){
            rotMap["right"] = nbs[n];
        }
        //left
        else if(nbs[n]->getXY() - h->getXY() == QPoint(-1,0)){
            rotMap["left"] = nbs[n];
        }
        //right up
        else if(nbs[n]->getXY() - h->getXY() == QPoint(1,1)){
            rotMap["rightUp"] = nbs[n];
        }
        //right down
        else if(nbs[n]->getXY() - h->getXY() == QPoint(0,-1)){
            rotMap["rightDown"] = nbs[n];
        }
        //left up
        else if(nbs[n]->getXY() - h->getXY() == QPoint(0,1)){
            rotMap["leftUp"] = nbs[n];
        }
        //leftDown
        else if(nbs[n]->getXY() - h->getXY() == QPoint(-1,-1)){
            rotMap["leftDown"] = nbs[n];
        }
    }
    //顺时钟旋转
    TextBlock::switchWord(rotMap["right"],rotMap["rightDown"]);
    TextBlock::switchWord(rotMap["right"],rotMap["leftDown"]);
    TextBlock::switchWord(rotMap["right"],rotMap["left"]);
    TextBlock::switchWord(rotMap["right"],rotMap["leftUp"]);
    TextBlock::switchWord(rotMap["right"],rotMap["rightUp"]);
}
bool Gamesys::AntiRotate(HexTextBlock *h)
{
    if(h->atBorder()){
        return false;
    }

    QList<HexTextBlock*>& nbs = h->getNeightBors();
    QMap<QString,HexTextBlock*> rotMap;

    for (int n = 0; n < nbs.size(); ++n) {
        //right
        if(nbs[n]->getXY() - h->getXY() == QPoint(1,0)){
            rotMap["right"] = nbs[n];
        }
        //left
        else if(nbs[n]->getXY() - h->getXY() == QPoint(-1,0)){
            rotMap["left"] = nbs[n];
        }
        //right up
        else if(nbs[n]->getXY() - h->getXY() == QPoint(1,1)){
            rotMap["rightUp"] = nbs[n];
        }
        //right down
        else if(nbs[n]->getXY() - h->getXY() == QPoint(0,-1)){
            rotMap["rightDown"] = nbs[n];
        }
        //left up
        else if(nbs[n]->getXY() - h->getXY() == QPoint(0,1)){
            rotMap["leftUp"] = nbs[n];
        }
        //leftDown
        else if(nbs[n]->getXY() - h->getXY() == QPoint(-1,-1)){
            rotMap["leftDown"] = nbs[n];
        }
    }

    //逆时针旋转：12 23 34 45 56
    TextBlock::switchWord(rotMap["right"],rotMap["rightUp"]);
    TextBlock::switchWord(rotMap["right"],rotMap["leftUp"]);
    TextBlock::switchWord(rotMap["right"],rotMap["left"]);
    TextBlock::switchWord(rotMap["right"],rotMap["leftDown"]);
    TextBlock::switchWord(rotMap["right"],rotMap["rightDown"]);
    return true;
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

    for (TextBlock* t : selectedBlockMap) {
        t->hideBorder();
    }
    selectedBlockMap.clear();
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

    for (int n = 0; n < textBlocks.size(); ++n) {
        if(textBlocks[n]->getXY() == record.startXY){
            start = textBlocks[n];
        }
        if(textBlocks[n]->getXY() == record.toXY){
            dest = textBlocks[n];
        }
    }

    //undoRecords储存的是player变换的起点/终点，不需要反向操作。
    selectAndSwitch(record.type,start,dest);

    //加回到playerRecord
    addRecord(record);

    //useStep还原
    levelscene->setStep(++useStep);

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
    if(record.type != TranslateIcons::Type::ROTATE){
        selectAndSwitch(record.type,oldDest,oldStart);
    }
    //rotate变换直接顺时钟复原
    else{
        NRotate(static_cast<HexTextBlock*>(oldStart));
    }

    //加到undorecords供do操作使用。
    undoRecords.append(GameRecord(record.type,record.startXY,record.toXY),false);
    levelscene->doBtn->setOpacity(1);

    levelscene->setStep(--useStep);

    //检查undo是否还可用
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
    QString str("");

    if(currentMode == CLASSIC){
        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                str.append(textBlocks[x + y*cols]->getWord());
            }
        }
    }else if(currentMode == HEX){

        int initX = 0;
        int blockNum = radius;

        //将现有文字方块排列转换为QString的算法
        for (int y = radius-1; y >= 1-radius; --y) {
            //前半
            if(y > 0){
                //x由左往右 从radius-1-y 到
                for (int x = initX; x < initX + blockNum; ++x) {
                    QPoint pos(x,y);
                    str.append(findBlockAt(pos)->getWord());
                }
                --initX;
                ++blockNum;
            }
            //最长行+后半
            else if(y <= 0){
                for (int x = initX; x < initX + blockNum; ++x) {
                    QPoint pos(x,y);
                    str.append(findBlockAt(pos)->getWord());
                }
                --blockNum;
            }
        }
    }


    if(str == contents){
        completeGame();
    }
}


void Gamesys::completeGame()
{
    QString finishTime = MyAlgorithms::timeInterval(startTiming,QTime::currentTime());

    completePage->setContent(useStep,finishTime);
    tempview->setScene(completePage);
    Animation::TempPagein(view,tempview)->start(QAbstractAnimation::DeleteWhenStopped);

    //记录在排行文件
    RankRecord r(QDate::currentDate(),useStep,QTime::fromString(finishTime,"HH:mm:ss"));
    addRankRecord(r);
}

//要一个可选 打勾 或 x 的对话框。








