#include "textblock.h"
#include "myalgorithms.h"


RectTextBlock::RectTextBlock(QPoint xypos, QString w, QGraphicsItem *parent)
    :GameObject(parent),
    xy(xypos)
{
    setAcceptHoverEvents(true);

    QPixmap pixmap(":/item/src/item/wordBlock.png");
    pixmap.scaled(Settings::textBlockSize,Settings::textBlockSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(pixmap);

    word = new QGraphicsTextItem(w,this);

    //font
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/dinglieciweifont20250217-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,Settings::textBlockSize * 0.4);
    word->setFont(font);
    word->moveBy(5,
                 (boundingRect().height() - word->boundingRect().height())/2);

    //goldRect
    border = new QGraphicsRectItem(boundingRect(),this);
    border->setPen(QPen(QColor(0,170,255),4));
    border->hide();

}

RectTextBlock::~RectTextBlock()
{
    qDebug() << "RectBlock:" << word->toPlainText() << "has been delete!";
}

void RectTextBlock::reset()
{
    setxy({0,0});
    setWord("");
    border->hide();
}

void RectTextBlock::setWord(QString newWord)
{
    word->setPlainText(newWord);
}

QString RectTextBlock::getWord()
{
    return word->toPlainText();
}

void RectTextBlock::setxy(QPoint p)
{
    xy = p;
}

void RectTextBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
    effect->setColor(Qt::black);
    effect->setStrength(0.4);
    setGraphicsEffect(effect);
}

void RectTextBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setGraphicsEffect(nullptr);
}

void RectTextBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
}


void RectTextBlock::showBorder()
{
    border->show();
    //qDebug() << Word() << "is selected";
}

void RectTextBlock::hideBorder()
{
    border->hide();
    //qDebug() << Word() << "is Unselected";
}


QPoint RectTextBlock::getXY()
{
    return xy;
}






HexTextBlock::HexTextBlock(int size, QString w, QGraphicsItem *parent)
    :Hex(size,parent),
    isPainted(false),
    R(0)
{
    setAcceptHoverEvents(true);


    // 创建六边形路径
    word = new QGraphicsTextItem(w,this);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/dinglieciweifont20250217-2.ttf",word,QColor(137,207,240),18);
    word->moveBy(-15,-15);

    //border
    border = new QGraphicsPolygonItem(this);
    border->setPolygon(MyAlgorithms::makeHex(this->size));
    border->setPen(QPen(QColor(137,207,240),4));
    border->hide();

}

HexTextBlock::~HexTextBlock()
{
    qDebug() << "hexblock:" << word->toPlainText() << "has been delete!";
}


void HexTextBlock::reset()
{
    setWord("");
    border->hide();
    neighbors.clear();
    isPainted = false;
}

void HexTextBlock::addNeightBor(TextBlock *n)
{
    neighbors.append(static_cast<HexTextBlock*>(n));
}

QList<HexTextBlock *> &HexTextBlock::getNeightBors()
{
    return neighbors;
}


void HexTextBlock::setWord(QString newWord)
{
    word->setPlainText(newWord);
}

QString HexTextBlock::getWord()
{
    return word->toPlainText();
}


void HexTextBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
    effect->setColor(Qt::white);
    effect->setStrength(0.3);
    setGraphicsEffect(effect);
}

void HexTextBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setGraphicsEffect(nullptr);
}

void HexTextBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
}


void HexTextBlock::showBorder()
{
    border->show();
    qDebug() << getWord() << "is selected";
}

void HexTextBlock::hideBorder()
{
    border->hide();
    qDebug() << getWord() << "is Unselected";
}

QPoint HexTextBlock::getXY()
{
    return hpos;
}






void HexTextBlock::switchWord(HexTextBlock* h1,HexTextBlock* h2)
{
    QString temp = h1->getWord();
    h1->setWord(h2->getWord());
    h2->setWord(temp);

    qDebug() << h1->getWord() << "与" << h2->getWord() << " 交换了。";
}

void HexTextBlock::setHpos(QPoint newHpos)
{
    hpos = newHpos;
}

void HexTextBlock::setR(int nR)
{
    R = nR;
}

int HexTextBlock::getR()
{
    return R;
}


bool HexTextBlock::getIsPainted() const
{
    return isPainted;
}

void HexTextBlock::setIsPainted(bool newIsPainted)
{
    isPainted = newIsPainted;
}

bool HexTextBlock::atBorder()
{
    return neighbors.size() < 6;
}


void TextBlock::switchWord(TextBlock *t1, TextBlock *t2)
{
    QString temp = t1->getWord();
    t1->setWord(t2->getWord());
    t2->setWord(temp);

    qDebug() << t1->getWord() << "与" << t2->getWord() << " 交换了。";
}
