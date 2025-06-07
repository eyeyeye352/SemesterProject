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

QFont MyAlgorithms::getFontByPath(QString fontsrc,int size)
{
    // 加载字体
    int fontId = QFontDatabase::addApplicationFont(fontsrc);
    if (fontId == -1) {
        qDebug() << "font loaded failed!";
        return QFont();
    }
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    return QFont(fontFamily, size);
}

QPolygonF MyAlgorithms::makeHex(int size)
{
    QPolygonF hexagon;
    for(int i = 0; i < 6; ++i) {
        // 当前角度（弧度制）
        double angle =  M_PI/6 + M_PI / 3 * i;  // 60°递增

        // 计算顶点坐标
        float x = size * cos(angle);
        float y = size * sin(angle);

        hexagon << QPointF(x, y);
    }

    return hexagon;
}

//传入文件路径，返回整个内容。
QString MyAlgorithms::getContentInFile(QString filepath)
{
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)){
        return "";
    };


    QString text = file.readAll();
    file.close();
    return text;
}

QList<QPoint> MyAlgorithms::makeHexCoordi(int radius)
{

    QList<QPoint> hposList;
    //y从radius-1 到 1-radius
    int initX = 0;
    int blockNum = radius;

    for (int y = radius-1; y >= 1-radius; --y) {
        //前半
        if(y > 0){
            //x由左往右 从radius-1-y 到
            for (int x = initX; x < initX + blockNum; ++x) {
                hposList.append(QPoint(x,y));
            }
            --initX;
            ++blockNum;
        }
        //最长行+后半
        else if(y <= 0){
            for (int x = initX; x < initX + blockNum; ++x) {
                hposList.append(QPoint(x,y));
            }
            --blockNum;
        }
    }

    return hposList;
}

QString MyAlgorithms::timeInterval(QTime t1, QTime t2)
{
    //time——spending
    int msecs = abs(t1.msecsTo(t2));
    return QTime::fromMSecsSinceStartOfDay(msecs).toString("HH:mm:ss");
}



//预备好的存档文件格式
QString MyAlgorithms::saveTextOutPut = "savetime=%1\n"
                                       "mode=%2\n"
                                        "levelnum=%3\n"
                                       "time_spending=%4\n"
                                       "gamerecords=%5\n";
