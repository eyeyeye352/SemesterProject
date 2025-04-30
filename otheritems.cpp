#include "otheritems.h"


DragableObj::DragableObj(QPixmap pixmap, QGraphicsItem *parent)
    :GameObject(pixmap,parent),isDragging(false),
    onlyDragX(false),onlyDragY(false),
    range(0,0,Settings::screenWidth,Settings::screenHeight)
{
    setAcceptHoverEvents(true);
}

void DragableObj::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(contains(event->pos())){
        isDragging = true;
        mouseOriginPos = event->pos();
    }
}

void DragableObj::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isDragging){
        QPointF dir = event->pos() - mouseOriginPos;

        if(onlyDragX == onlyDragY){
            moveBy(dir.x(),dir.y());
        }
        else if(onlyDragX){
            moveBy(dir.x(),0);
        }
        else if(onlyDragY){
            moveBy(0,dir.y());
        }

        qDebug() << "draggingbtn rect:" << sceneBoundingRect();


        //out of range
        if(this->sceneBoundingRect().x() < range.x()){
            setX(range.x() + 2);
        }
        if(this->sceneBoundingRect().y() < range.y()){
            setY(range.y());
        }
        if(this->sceneBoundingRect().right() > range.right()){
            setX(range.right() - 2 - this->sceneBoundingRect().width());
        }
        if(this->sceneBoundingRect().bottom() > range.bottom()){
            setY(range.bottom() - this->sceneBoundingRect().height());
        }
    }
}

void DragableObj::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isDragging = false;
}

void DragableObj::setOnlyDragX(bool newOnlyDragX)
{
    onlyDragX = newOnlyDragX;
}

void DragableObj::setOnlyDragY(bool newOnlyDragY)
{
    onlyDragY = newOnlyDragY;
}

void DragableObj::setRange(QRectF newRange)
{
    range = newRange;
}



//黑幕
BlackOverlay::BlackOverlay(QGraphicsItem *parent):
    GameObject{QPixmap(":/background/src/background/BlackOverLay.png"),parent}
{
    setOpacity(0);
}




ValSets::ValSets(QGraphicsItem *parent):
    GameObject(parent),
    upBtn(new FunctionBtn(QPixmap(":/item/src/item/valSetsBtn.png"),this)),
    downBtn(new FunctionBtn(QPixmap(":/item/src/item/valSetsBtn.png"),this)),
    val(1)
{


    upBtn->setOpacity(1);

    valText = new QGraphicsTextItem(QString::number(val*10),this);
    //style
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/XiangJiaoKuanMaoShuaLingGanTi-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,25);
    valText->setFont(font);
    valText->setDefaultTextColor(Qt::white);
    valText->setPos(upBtn->boundingRect().right() ,
                    (upBtn->boundingRect().height() - valText->boundingRect().height())/2);


    downBtn->setOpacity(1);
    downBtn->setPos(upBtn->boundingRect().width()+valText->boundingRect().width() ,
                    upBtn->boundingRect().y());
    downBtn->setTransformOriginPoint(downBtn->boundingRect().center());
    downBtn->setRotation(180);



    connect(upBtn,&GameBtn::clicked,[this]{
        if(val + 0.1 <= 1){
            val+=0.1;
        }
        valText->setPlainText(QString::number(val*10,'f',0));
        emit valueChanged(val);
    });
    connect(downBtn,&GameBtn::clicked,[this]{
        if(val - 0.1 > 0){
            val-=0.1;
        }

        valText->setPlainText(QString::number(val*10,'f',0));

        emit valueChanged(val);
    });
}


QRectF ValSets::boundingRect() const
{
    QRectF rect(0,0,
                2*upBtn->boundingRect().width() + valText->boundingRect().width(),
                valText->boundingRect().height());
    return rect;
}
