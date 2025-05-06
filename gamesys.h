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

    //打开，关闭设置
    void openSetting();
    void backHome();

    //save
    void openSavePage();

    void closeTempPage();

    //tips
    void openTips();

    //rank
    void checkRank();

    //creation mode
    void goCreateMode();

    //选择关卡后进入游戏场景(先动画，动画到一半开始加载),参数为第几关
    void loadGameAnime(int levelNum);
    void loadGame(int levelNum);

    //游戏通关
    void completeGame();



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
    Mode currentMode;




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
    bool selectAndSwitch(TranslateIcons::Type type,TextBlock* start,TextBlock* dest);

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
    TextBlock* getRandBlock();
    TextBlock* getRandBlockInCross();


private:

    int rows,cols;

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
