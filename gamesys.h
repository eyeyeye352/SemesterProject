#ifndef GAMESYS_H
#define GAMESYS_H

#include "scene.h"
#include "settings.h"
#include "objpool.h"
#include "gamerecord.h"




class Gamesys : public QWidget
{
    Q_OBJECT

public:
    Gamesys(QWidget *parent = nullptr);
    ~Gamesys();

public slots:

    //进入关卡选择页面
    void goLevelSelection(Mode);

    //返回模式选择页面
    void backModeSelection();

    //打开，关闭temppage
    void openSettingPage();
    void openSavePage();
    void openTipPage();
    void openRankPage();
    void completeGame();

    void goHome();
    void closeTempPage();


    //creation mode
    void goCreateMode();

    //选择关卡后进入游戏场景(先动画，动画到一半开始加载),参数为第几关
    void loadGameAnime(int levelNum,bool shuffled);
    void loadGame(int levelNum,bool shuffled);

    //load过程用：读取difficulty信息后，classsic模式就计算行列，hex模式计算大六边形边长
    void readDifficulty(QString difficulty_line);

    //load过程用，根据模式加载textBlock或hexTextBlock
    void loadTextBlocks();

    //save&load 相关功能
    //初始加载slot信息
    void initSLSlot();
    //集成的函数，接收slot点击事件执行以下对应功能：
    void processSlots(int slotNum,int state);
    //savestate 为 save时，点击slot存档。
    void saveSlotAt(int slotNum);
    //直接加载saveload记录好的关卡进度
    void loadSaveGame(SaveSlot* s);


private:
    StartScene* startscene;
    LevelScene* levelscene;
    SettingPage* settingPage;
    TipPage* tipPage;
    RankPage* rankPage;
    SavePage* savePage;
    CompletePage* completePage;

    QGraphicsView* view , *tempview;

    //timers
    QTimer * bgmoveTimer;

    //current gameMode
    int currentMode;




public slots:
    //核心玩法：选中blocks/交换blocks
    void selectBlocks(TextBlock*);
    void switchBlocks(TextBlock*);
    void changeTransType(TranslateIcons::Type type);
    void cancelSelect();

    void DoTrans();
    void UndoTrans();

public:

    //洗牌出题
    void shuffleLevel();

    //指定模式+选取+变换一起，用于开场洗牌/do/undo,返回值表示是否变换成功。
    bool selectAndSwitch(int type,TextBlock* start,TextBlock* dest);

    //用户每次操作后，添加记录
    void addRecord(GameRecord record);

    //每次操作后，检查是否通关
    void checkIfComplete();

    void resetLevel();

    //选取/变换细项
    void rowSelect(TextBlock*);
    void colSelect(TextBlock*);
    void crossSelect(TextBlock*);

    //(变换成功与否)
    bool rowSwitch(TextBlock*);
    bool colSwitch(TextBlock*);
    bool crossSwitch(TextBlock*);


    //返回随机一个textBlock/随机一个不在边缘地带的Block
    RectTextBlock* getRandBlock();
    RectTextBlock* getRandBlockInCross();
    RectTextBlock* findBlockAt(QPoint target);


private:

    //classic模式
    int rows,cols;
    //hex模式
    int radius;

    //内容，同时作为解答
    QString contents;

    //根据当前是否有选中的block执行select或switch
    bool hasSelects;
    TranslateIcons::Type curTransType;

    //变换用容器
    QList<TextBlock*> selectedBlocks;
    QMap<QString,TextBlock*> crossTransMap;

    QList<TextBlock*> textBlocks;

    //gamerecords(sys为系统洗牌打乱的记录，player为玩家操作记录,undo时会弹栈，do会压栈)
    GameRecordList playerRecords;
    //记录所有undo的操作
    GameRecordList undoRecords;
    GameRecordList sysRecord;

    //临时记录用的textBlock指针
    TextBlock* temp;

    //玩家使用的步数
    int useStep;

    //玩家当前进行的关卡
    int curLevelNum;

};
















#endif // GAMESYS_H
