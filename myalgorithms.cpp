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

//预备好的存档文件格式
QString MyAlgorithms::saveTextOutPut = "savetime=%1\n"
                                       "mode=%2\n"
                                        "levelnum=%3\n"
                                       "gamerecords=%4\n";
