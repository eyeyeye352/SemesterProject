#ifndef MYALGORITHMS_H
#define MYALGORITHMS_H

#include "settings.h"
#include "textblock.h"

struct MyAlgorithms
{
    MyAlgorithms() = delete;

    //为指定textitem设置字体、颜色、大小。
    static void addFontToTextItem(QString fontsrc,QGraphicsTextItem* item,QColor textDefaultColor,int fontsize);

    static QString saveTextOutPut;
};

#endif // MYALGORITHMS_H
