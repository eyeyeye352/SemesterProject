#ifndef GAMERECORD_H
#define GAMERECORD_H

#include "gameobject.h"

//一条操作记录保存：使用的变换模式，选择位置，变换位置
struct GameRecord
{
    GameRecord(TranslateIcons::Type t,QPoint sXY,QPoint tXY):
        type(t),startXY(sXY),toXY(tXY)
    {
        qDebug() << "rocord: " << type << startXY << " to " << toXY;
    }

    TranslateIcons::Type type;
    QPoint startXY;
    QPoint toXY;
};

#endif // GAMERECORD_H
