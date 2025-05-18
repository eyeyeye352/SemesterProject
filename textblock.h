#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include "settings.h"
#include "gameobject.h"
#include "hex.h"

//关卡内的文字方块元素

class TextBlock{
public:
    virtual ~TextBlock(){}
    virtual void reset() = 0;
    virtual void setWord(QString) = 0;
    virtual QString getWord() = 0;
    virtual void showBorder() = 0;
    virtual void hideBorder() = 0;
    virtual QPoint getXY() = 0;

    //static函数，交换两个方块的文字
    static void switchWord(TextBlock* t1,TextBlock* t2);

protected:
    QGraphicsTextItem* word;
};



class RectTextBlock : public GameObject, public TextBlock
{
    Q_OBJECT
public:
    RectTextBlock(QPoint xypos,QString word,QGraphicsItem *parent);
    ~RectTextBlock();

    void reset() override;
    void setWord(QString newWord) override;
    QString getWord() override;
    void setxy(QPoint);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void showBorder() override;
    void hideBorder() override;
    QPoint getXY() override;


signals:
    void clicked(RectTextBlock*);
private:

    QPoint xy;

    //根据变换模式，被选中标记(金色框框)
    QGraphicsRectItem* border;

};




class HexTextBlock : public Hex, public TextBlock
{
    Q_OBJECT
public:
    HexTextBlock(int size,QString w,QGraphicsItem *parent);
    ~HexTextBlock();

    void reset() override;
    void addNeightBor(TextBlock* n);
    QList<HexTextBlock*>& getNeightBors();

    void setWord(QString newWord) override;
    QString getWord() override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void showBorder() override;
    void hideBorder() override;
    QPoint getXY() override;


    //static函数，交换两个方块的文字
    static void switchWord(HexTextBlock* h1,HexTextBlock* h2);

    void setHpos(QPoint newHpos);

    bool getIsPainted() const;
    void setIsPainted(bool newIsPainted);

    //返回是否位于边缘地带
    bool atBorder();

signals:
    void clicked(HexTextBlock*);
private:


    //根据变换模式，被选中标记(金色框框)
    QGraphicsPolygonItem* border;

    //六边形坐标系
    QPoint hpos;

    //周围的六边形
    QList<HexTextBlock*> neighbors;

    bool isPainted;
};



#endif // TEXTBLOCK_H
