#include "myalgorithms.h"

//一个通用的函数，他用于给指定的QGraphcistextitem设置字体
void MyAlgorithms::addFontToTextItem(QString fontsrc, QGraphicsTextItem *item,QColor c,int fontsize)
{
    int fontId = QFontDatabase::addApplicationFont(fontsrc);
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,fontsize);
    item->setFont(font);
    item->setDefaultTextColor(c);
}
