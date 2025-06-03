#ifndef TEMPPAGE_H
#define TEMPPAGE_H

#include "settings.h"
#include "scene.h"


//设置、排行、提示等页面的基类
class TempPage : public Scene{

    Q_OBJECT
public:
    TempPage(QObject *parent);

    TempPageBtn* addBtn(QString text,QGraphicsItem* btnParent = nullptr);

signals:
    //关闭页面信号，一般在back按钮被点击时发出，请求gamesys关闭该场景。
    void closePage();

protected:
    QList<TempPageBtn*> btns;
};




class SettingPage : public TempPage
{
    Q_OBJECT
public:
    SettingPage(QObject *parent = nullptr);

signals:
    void goHome();

private:

    QGraphicsPixmapItem* bg;
    ValSets *musicSli;
    ValSets *soundSli;
    QPointer<TempPageBtn> homeBtn;
    QPointer<TempPageBtn> backBtn;
};



class TipPage : public TempPage
{
    Q_OBJECT

public:
    TipPage(QObject *parent = nullptr);

    void setClassicAnswer(QString text,int cols);
    void setHexAnswer(QString text,int radius);
    void resetAnswer();

private:

    QGraphicsPixmapItem* bg;
    QGraphicsTextItem* answer;
    QPointer<TempPageBtn> backBtn;
};




class RankPage : public TempPage{

    Q_OBJECT
public:

    RankPage(QObject* parent);


    //通关时新增记录到rankrecord文件
    void showContents(QList<RankRecord>& rList);

signals:
    void orderByTime();
    void orderByStep();


private:
    QGraphicsPixmapItem* bg;
    QPointer<QTextEdit> text;
    QGraphicsTextItem* description;
    QPointer<TempPageBtn> backBtn , timeOrderBtn , stepOrderBtn;
};





class SavePage : public TempPage{

    Q_OBJECT

public:
    SavePage(QObject *parent = nullptr);

    void backToNoneState();
    void addBlack(int newState);
    void hideSaveBtn();
    void showSaveBtn();

    QList<SaveSlot*>& getSlots();

    //当前页面的选择状态，包括：
    //1. None，此时点击back会发出closeTempPage信号，点击save和load进入对应状态
    //2. load, 此时back会返回NONE状态，save和load禁用（背景变暗突出back和slots）
    //3. save，同理
    enum State{
        NONE,
        LOAD,
        SAVE
    };

signals:
    void slotSelected(int slotNum,int state);
    void saveAt(int slotNum);
    void loadAt(int slotNum);

private:


    QGraphicsPixmapItem* bg;

    //四个槽位
    QList<SaveSlot*> SLslots;
    TempPageBtn *back,*save,*load;

    //黑幕效果，state为LOAD或SAVE时凸显slot及back用
    QGraphicsRectItem* blackLay;
    int state;
};



//游戏通关场景
class CompletePage : public TempPage{

    Q_OBJECT
public:
    CompletePage(QObject *parent = nullptr);

    void setContent(int step,QString time_spending);

signals:
    void goHome();


private:

    QGraphicsPixmapItem* bg;
    QGraphicsTextItem * stepText;
    QPointer<TempPageBtn> homeBtn;

};

#endif // TEMPPAGE_H
