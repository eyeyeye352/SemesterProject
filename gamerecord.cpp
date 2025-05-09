#include "gamerecord.h"
#include "gameobject.h"


void GameRecordList::append(GameRecord r1,bool optim)
{
    //优化：相邻重复项抵消
    if(optim && !rList.isEmpty() && r1.type == rList.top().type){

        GameRecord r2 = rList.top();

        switch(r1.type){

        case TranslateIcons::ROWS:
            //两次变换的y相同时抵消
            if(r1.startXY.y() == r2.toXY.y() && r1.toXY.y() == r2.startXY.y()){
                return;
            }
            break;
        case TranslateIcons::COLS:
            if(r1.startXY.x() == r2.toXY.x() && r1.toXY.x() == r2.startXY.x()){
                return;
            }
            break;
        case TranslateIcons::CROSS:
            if(r1.startXY == r2.toXY && r1.toXY == r2.startXY){
                return;
            }
            break;
        case TranslateIcons::HEXLINE:
            break;
        case TranslateIcons::HEX:
            break;
        }
    }

    rList.append(r1);

}

GameRecord GameRecordList::pop()
{
    return rList.pop();
}

int GameRecordList::size()
{
    return rList.size();
}

bool GameRecordList::isEmpty()
{
    return rList.isEmpty();
}

void GameRecordList::clear()
{
    rList.clear();
}

GameRecord &GameRecordList::operator[](int n)
{
    return rList[n];
}

GameRecordList& SaveSlot::getSysRecords()
{
    return sysR;
}

GameRecordList& SaveSlot::getPlayerRecords()
{
    return playerR;
}
