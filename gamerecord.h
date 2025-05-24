#ifndef GAMERECORD_H
#define GAMERECORD_H

#include "settings.h"

//一条操作记录保存：使用的变换模式，选择位置，变换位置
struct GameRecord
{
    GameRecord() = default;

    GameRecord(int t,QPoint sXY,QPoint tXY):
        type(t),startXY(sXY),toXY(tXY)
    {}

    void info(){
        qDebug() << "record type: " << type << "from" << startXY << "to" << toXY;
    }

    int type;
    QPoint startXY;
    QPoint toXY;
};



//自己写的优化过的GameRecordList
struct GameRecordList{

    void append(GameRecord r,bool optimize);
    GameRecord pop();
    int size();
    bool isEmpty();
    void clear();

    GameRecord& operator[](int n);

private:
    QStack<GameRecord> rList;
};



//排行的记录
struct RankRecord{
    RankRecord() = default;
    RankRecord(QDate d,int step,QTime ts):
        save_date(d),step_using(step),time_spending(ts){}
    RankRecord(QString line);

    QString toString();

    QDate save_date;
    int step_using;
    QTime time_spending;
};


#endif // GAMERECORD_H
