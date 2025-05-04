#include "textblock.h"


TextBlock::TextBlock(QPoint xypos, QString w, QGraphicsItem *parent)
    :GameObject(parent),
    xy(xypos),
    isClicked(false),
    target(false)
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
    goldRect = new QGraphicsRectItem(boundingRect(),this);
    goldRect->setPen(QPen(QColor(0,170,255),4));
    goldRect->hide();

}

TextBlock::~TextBlock()
{
    qDebug() << "textblock:" << word->toPlainText() << "has been delete!";
}

void TextBlock::setWord(QString newWord)
{
    word->setPlainText(newWord);
}

QString TextBlock::Word()
{
    return word->toPlainText();
}

void TextBlock::setxy(QPoint p)
{
    xy = p;
}

void TextBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
    effect->setColor(Qt::black);
    effect->setStrength(0.4);
    setGraphicsEffect(effect);
}

void TextBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setGraphicsEffect(nullptr);
}

void TextBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
}

void TextBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

bool TextBlock::isTarget()
{
    return target;
}

void TextBlock::setTarget(bool b)
{
    target = b;
    target ? goldRect->show() : goldRect->hide();
    if(target) qDebug() << Word() << "is selected";
}

QPoint TextBlock::getXY()
{
    return xy;
}
