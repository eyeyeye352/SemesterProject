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


#endif // GAMERECORD_H
