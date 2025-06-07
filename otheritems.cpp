#include "otheritems.h"
#include "myalgorithms.h"

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




ValSets::ValSets(QGraphicsItem *parent, int Max, int Min):
    GameObject(parent),
    upBtn(new FunctionBtn(QPixmap(":/item/src/item/valSetsBtn.png"),this)),
    downBtn(new FunctionBtn(QPixmap(":/item/src/item/valSetsBtn.png"),this)),
    val(Max),max(Max),min(Min)
{


    valText = new QGraphicsTextItem(QString::number(val),this);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/XiangJiaoKuanMaoShuaLingGanTi-2.ttf",valText,Qt::white,25);
    valText->setPos(upBtn->boundingRect().right() ,
                    (upBtn->boundingRect().height() - valText->boundingRect().height())/2);


    downBtn->setOpacity(1);
    downBtn->setPos(upBtn->boundingRect().width()+valText->boundingRect().width() ,
                    upBtn->boundingRect().y());
    downBtn->setTransformOriginPoint(downBtn->boundingRect().center());
    downBtn->setRotation(180);



    connect(upBtn,&GameBtn::clicked,[this]{
        if(val + 1 <= max){
            ++val;
        }else{
            val = max;
        }
        valText->setPlainText(QString::number(val));
        emit valueChanged(val);
    });
    connect(downBtn,&GameBtn::clicked,[this]{

        if(val - 1 >= min){
            --val;
        }else{
            val = min;
        }
        valText->setPlainText(QString::number(val));
        emit valueChanged(val);
    });
}

ValSets::~ValSets()
{
    qDebug() << "valsets delete!";
}


QRectF ValSets::boundingRect() const
{
    QRectF rect(0,0,
                2*upBtn->boundingRect().width() + valText->boundingRect().width(),
                valText->boundingRect().height());
    return rect;
}

int ValSets::getVal()
{
    return val;
}

void ValSets::setBound(int Max, int Min)
{
    max = Max;
    min = Min;
    if(val < Min || val > Max){
        val = Max;
        valText->setPlainText(QString::number(val));
    }
}

ModeValSets::ModeValSets(QGraphicsItem *parent):
    ValSets{parent}
{
    valText->setOpacity(0);
    text = new QGraphicsTextItem("Classic",this);
    text->setPos(upBtn->boundingRect().right() ,
                 (upBtn->boundingRect().height() - text->boundingRect().height())/2);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/XiangJiaoKuanMaoShuaLingGanTi-2.ttf",text,Qt::white,25);

    downBtn->setPos(upBtn->boundingRect().width()+text->boundingRect().width() ,
                    upBtn->boundingRect().y());

    disconnect(upBtn,&GameBtn::clicked,nullptr,nullptr);
    disconnect(downBtn,&GameBtn::clicked,nullptr,nullptr);


    connect(upBtn,&GameBtn::clicked,[this]{
        if(text->toPlainText() == "Classic"){
            text->setPlainText("Hex");
            emit modeChanged(Mode::HEX);
        }

    });
    connect(downBtn,&GameBtn::clicked,[this]{
        if(text->toPlainText() == "Hex"){
            text->setPlainText("Classic");
            emit modeChanged(Mode::CLASSIC);
        }
    });
}

ModeValSets::~ModeValSets()
{}

int ModeValSets::getMode()
{
    if(text->toPlainText() == "Classic"){
        return Mode::CLASSIC;
    }else{
        return Mode::HEX;
    }
}
