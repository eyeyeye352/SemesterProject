#ifndef MYALGORITHMS_H
#define MYALGORITHMS_H

#include "settings.h"
#include "textblock.h"

struct MyAlgorithms
{
    MyAlgorithms() = delete;

    //为指定textitem设置字体、颜色、大小。
    static void addFontToTextItem(QString fontsrc,QGraphicsTextItem* item,QColor textDefaultColor,int fontsize);

    //传入路径获取字体
    static QFont getFontByPath(QString fontsrc,int size);

    static QString saveTextOutPut;

    //传入size，生成一个对应的六边形
    static QPolygonF makeHex(int size);

    //传入文件路径，返回整个文本
    static QString getContentInFile(QString filapath);

    //生成一个六边形坐标系，（从左上角开始，往上到下，从左到右）
    static QList<QPoint> makeHexCoordi(int radius);

    //传入两个时间，将其转换为qstring（hh：mm：ss）
    static QString timeInterval(QTime t1,QTime t2);
};

#endif // MYALGORITHMS_H
