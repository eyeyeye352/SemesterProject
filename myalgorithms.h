#ifndef MYALGORITHMS_H
#define MYALGORITHMS_H

#include "settings.h"

struct MyAlgorithms
{
    MyAlgorithms() = delete;

    static void addFontToTextItem(QString fontsrc,QGraphicsTextItem* item,QColor textDefaultColor,int fontsize);
};

#endif // MYALGORITHMS_H
