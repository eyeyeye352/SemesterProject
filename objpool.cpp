#include "objpool.h"

objPool* objPool::instance = nullptr;
objPool::objPool() {
    //120个
    for (int n = 0; n < 120; ++n) {
        textblockPool.append(new TextBlock);
        qDebug() << "崩溃点？" << n;
    }
}

objPool::~objPool()
{
    for (int n = 0; n < textblockPool.size(); ++n) {
        delete textblockPool[n];
    }
    textblockPool.clear();
}

TextBlock *objPool::getTextBlock()
{
    if(!textblockPool.isEmpty()){
        TextBlock* t = textblockPool.front();
        textblockPool.removeFirst();
        return t;
    }
    else{
        qDebug() << "pool不够用了！";
        return nullptr;
    }

}

void objPool::recycle(TextBlock * tb)
{
    textblockPool.append(tb);
    //qDebug() << "recycle success";
}
