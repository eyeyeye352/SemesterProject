#ifndef LEVELS_H
#define LEVELS_H

#include "settings.h"
#include "musicplayer.h"

class LevelBlock :  public QObject , public QGraphicsPixmapItem{

    Q_OBJECT
    // 声明 opacity 属性
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)

public:

    LevelBlock(int l,QGraphicsItem *parent = nullptr);

    //载入文件记录关卡信息
    void loadFile(QString);
    QString getInfo() const;

    //鼠标悬停特效
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    //点击效果
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    //被选中的关卡
    void selected(int);

private:
    //哪一关
    int levelNum;
    //关卡详细信息
    QString info;
};

#endif // LEVELS_H
