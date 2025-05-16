#include "objpool.h"

objPool* objPool::instance = nullptr;
objPool::objPool() {
    //120个
    for (int n = 0; n < 120; ++n) {
        rectPool.append(new RectTextBlock(QPoint(0,0),"",nullptr));
        hexPool.append(new HexTextBlock(0,"",nullptr));
    }
    qDebug() << "对象池初始化成功";
}

objPool::~objPool()
{
    for (int n = 0; n < rectPool.size(); ++n) {
        delete rectPool[n];
    }
    for(int n = 0; n < hexPool.size(); ++n){
        delete hexPool[n];
    }
    qDebug() << "对象池析构！";
}

RectTextBlock * objPool::getRectTextBlock()
{
    if(!rectPool.isEmpty()){
        RectTextBlock* t = rectPool.front();
        rectPool.removeFirst();
        return t;
    }
    else{
        qDebug() << "pool不够用了！";
        return nullptr;
    }

}

HexTextBlock *objPool::getHexTextBlock()
{
    if(!hexPool.isEmpty()){
        HexTextBlock* h = hexPool.front();
        hexPool.removeFirst();
        return h;
    }
    else{
        qDebug() << "pool不够用了！";
        return nullptr;
    }
}

void objPool::recycle(RectTextBlock * t)
{
    rectPool.append(t);
    qDebug() << t->getWord() << "被回收.";
    t->reset();
}

void objPool::recycle(HexTextBlock *h)
{
    hexPool.append(h);
    qDebug() << h->getWord() << "被回收.";
    h->reset();
}
