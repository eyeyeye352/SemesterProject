#include "textblock.h"



TextBlock::TextBlock(QGraphicsItem *parent)
    : GameObject{parent}
{
    QPixmap pixmap(":/item/src/item/wordBlock.png");
    pixmap.scaled(Settings::textBlockSize,Settings::textBlockSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(pixmap);
}

TextBlock::TextBlock(QPoint xypos, QString w, QGraphicsItem *parent)
    :GameObject(parent),xy(xypos)
{
    QPixmap pixmap(":/item/src/item/wordBlock.png");
    pixmap.scaled(Settings::textBlockSize,Settings::textBlockSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    setPixmap(pixmap);

    word = new QGraphicsTextItem(w);
    //style
    int fontId = QFontDatabase::addApplicationFont(":/fonts/src/fonts/dinglieciweifont20250217-2.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(fontFamily,Settings::textBlockSize * 0.5);
    word->setFont(font);
    word->setParentItem(this);
    //微调位置
    word->moveBy(3,2);


}

void TextBlock::setWord(QString newWord)
{
    word->setPlainText(newWord);
}
