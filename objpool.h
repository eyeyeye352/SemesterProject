#ifndef OBJPOOL_H
#define OBJPOOL_H

#include "textblock.h"
#include "hex.h"
#include "settings.h"

class objPool
{
    objPool();
    ~objPool();
    static objPool* instance;
public:
    static objPool* getinstance(){
        if(instance == nullptr){
            instance = new objPool;
        }
        return instance;
    }

    static void delinstance(){
        if(instance != nullptr){
            delete instance;
            instance = nullptr;
        }
    }

    void init();

    RectTextBlock* getRectTextBlock();
    HexTextBlock* getHexTextBlock();

    void recycle(RectTextBlock*);
    void recycle(HexTextBlock*);

private:
    QList<RectTextBlock*> rectPool;
    QList<HexTextBlock*> hexPool;

};

#endif // OBJPOOL_H
