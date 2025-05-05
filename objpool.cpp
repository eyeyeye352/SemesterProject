#include "objpool.h"

objPool* objPool::instance = nullptr;
objPool::objPool() {
    //120个
    for (int n = 0; n < 120; ++n) {
        textblockPool.append(new TextBlock);
    }
    qDebug() << "对象池textblocks数量：" << textblockPool.size();
}

objPool::~objPool()
{
    for (int n = 0; n < textblockPool.size(); ++n) {
        delete textblockPool[n];
    }
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

void objPool::recycle(TextBlock * t)
{
    textblockPool.append(t);
    t->reset();

    //qDebug() << "recycle success";
}
