#include "gameobject.h"
#include "musicplayer.h"
#include "otheritems.h"
#include "myalgorithms.h"




GameObject::GameObject(QGraphicsItem *parent):QGraphicsPixmapItem{parent}{}

GameObject::GameObject(QPixmap pixmap, QGraphicsItem *parent)
    :QGraphicsPixmapItem{pixmap,parent}
{}

void GameObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
}


//按键可接受hover事件。
GameBtn::GameBtn(QPixmap pixmap, QGraphicsItem *parent)
    :GameObject(pixmap,parent)
{
    setAcceptHoverEvents(true);
}

//缩放并调整按键位置，保证中心不变
void GameBtn::resizing(double scale)
{
    QPointF center = sceneBoundingRect().center();
    setScale(scale);
    QPointF dir = (center - sceneBoundingRect().center());
    moveBy(dir.x(),dir.y());
}



//经典模式按钮
ClassicBtn::ClassicBtn(QGraphicsItem *parent):
    GameBtn{QPixmap(":/item/src/item/classic.png"),parent}
{
    resizing(Settings::startBtnOriginScale);
}


void ClassicBtn::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    MusicPlayer::getMPlayer()->startBtnSound();
    setPixmap(QPixmap(":/item/src/item/classic_hover.png"));
}

void ClassicBtn::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setPixmap(QPixmap(":/item/src/item/classic.png"));
}

void ClassicBtn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //模式按键大小调整
    resizing(Settings::startBtnOriginScale*0.9);
}

void ClassicBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::startBtnOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked(Mode::CLASSIC);
}


//hex模式按钮
HexBtn::HexBtn(QGraphicsItem *parent):
    GameBtn{QPixmap(":/item/src/item/hexagon.png"),parent}
{
    resizing(Settings::startBtnOriginScale);
}

void HexBtn::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    MusicPlayer::getMPlayer()->startBtnSound();
    setPixmap(QPixmap(":/item/src/item/hexagon_hover.png"));
}

void HexBtn::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setPixmap(QPixmap(":/item/src/item/hexagon.png"));
}

void HexBtn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::startBtnOriginScale*0.9);
}

void HexBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::startBtnOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked(Mode::HEX);
}




FunctionBtn::FunctionBtn(QPixmap pixmap, QGraphicsItem *parent):
    GameBtn{pixmap,parent}
{}


void FunctionBtn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::funcBtnOriginScale*0.9);
}

void FunctionBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::funcBtnOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit clicked();
}




LevelSelectBlock::LevelSelectBlock(int l, QGraphicsItem *parent)
    :GameBtn{QPixmap(":/item/src/item/levelBlock.png"),parent},
    levelNum(l)
{
    setScale(Settings::levelBlockOriginScale);

    QGraphicsTextItem * text = new QGraphicsTextItem(QString::number(levelNum),this);

    //关卡数字字体
    text->setFont(QFont("Tw Cen MT Condensed Extra Bold",20));
    text->setPos(
        (sceneBoundingRect().width() - text->sceneBoundingRect().width())/2,
        (sceneBoundingRect().height() - text->sceneBoundingRect().height())/2
        );

}



void LevelSelectBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsColorizeEffect* fx = new QGraphicsColorizeEffect(this);
    fx->setColor(Qt::black);
    fx->setStrength(0.8);
    setGraphicsEffect(fx);
}

void LevelSelectBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setGraphicsEffect(nullptr);
}

void LevelSelectBlock::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::levelBlockOriginScale*0.9);
}

void LevelSelectBlock::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(Settings::levelBlockOriginScale);
    MusicPlayer::getMPlayer()->clickSound();
    emit selected(levelNum);
}







TranslateIcons::TranslateIcons(QPixmap pixmap, Type t,QGraphicsItem *parent):
    GameObject(pixmap,parent),
    type(t)
{
    goldRect = new QGraphicsRectItem(boundingRect(),this);
    goldRect->setPen(QPen(QColor(255,215,0),2));
    goldRect->hide();
}

void TranslateIcons::makeGold()
{
    goldRect->show();
}

void TranslateIcons::removeGold()
{
    goldRect->hide();
}

void TranslateIcons::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit selected(type);
}















TempPageBtn::TempPageBtn(QGraphicsItem *parent):
    GameBtn(QPixmap(":/item/src/item/tempPageBtn.png"),parent)
{
    text = new QGraphicsTextItem(this);
    //font
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",text,Qt::white,15);
}

void TempPageBtn::setText(QString t)
{
    text->setPlainText(t);
    text->setPos((boundingRect().width() - text->boundingRect().width())/2,
                 (boundingRect().height() - text->boundingRect().height())/2);
}

void TempPageBtn::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(0.9);
}

void TempPageBtn::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    resizing(1);
    emit clicked();
}




SaveSlot::SaveSlot(QGraphicsItem *parent)
    :QGraphicsRectItem(parent)
{

    setRect(0,0,200,150);
    setPen(QPen(Qt::black,5));
    setBrush(QColor(0,150,0));

    text = new QGraphicsTextItem(this);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",text,Qt::white,16);

}


void SaveSlot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    setTransformOriginPoint(boundingRect().center());
    setScale(0.9);

}

void SaveSlot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setScale(1);
    setTransformOriginPoint(0,0);
    emit clicked();
}


void SaveSlot::init(QString filepath)
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);

    QString whole = file.readAll();
    QStringList lines = whole.split("\n");

    //重置保存的记录
    sysR.clear();
    playerR.clear();

    QString showtext("");

    for (int n = 0; n < lines.size(); ++n) {

        //时间
        if(lines[n].startsWith("savetime=")){
            savetime = lines[n].remove("savetime=");
            showtext.append(savetime + '\n');
        }

        //模式
        if(lines[n].startsWith("mode=")){
            gameMode = lines[n].remove("mode=").toInt();
            if(gameMode == CLASSIC){
                showtext.append("classic-");
            }else if(gameMode == HEX){
                showtext.append("hex-");
            }
        }

        //levelNum
        if(lines[n].startsWith("levelnum=")){
            levelNum = lines[n].remove("levelnum=").toInt();
            showtext.append(lines[n].remove("levelnum="));
        }



        //记录
        if(lines[n].startsWith("gamerecords=")){

            int recordnum = lines[n].remove("gamerecords=").toInt();
            qDebug() << "n:" << n << " recordnum" << recordnum;

            for (int N = n+1; N < n+1+recordnum && N < lines.size(); ++N) {

                //系统记录
                if(lines[N].startsWith("sys:")){

                    QStringList record = lines[N].remove("sys:").split(' ');

                    int transType = record[0].toInt();
                    QPoint start(record[1].toInt(),record[2].toInt());
                    QPoint dest(record[3].toInt(),record[4].toInt());

                    sysR.append(GameRecord(transType,start,dest),false);

                }else if(lines[N].startsWith("player:")){

                    QStringList record = lines[N].remove("player:").split(' ');

                    int transType = record[0].toInt();
                    QPoint start(record[1].toInt(),record[2].toInt());
                    QPoint dest(record[3].toInt(),record[4].toInt());

                    playerR.append(GameRecord(record[0].toInt(),start,dest),false);
                }
            }
        }

    }

    text->setPlainText(showtext);
    text->setPos((boundingRect().width() - text->boundingRect().width())/2,
                 (boundingRect().height() - text->boundingRect().height())/2);
}

void SaveSlot::info()
{

    qDebug() << savetime;
    qDebug() << "mode:" << gameMode << "level:" << levelNum;

    qDebug() << "sysR";
    for (int n = 0; n < sysR.size(); ++n) {
        sysR[n].info();
    }

    qDebug() << "playerR";
    for (int n = 0; n < playerR.size(); ++n) {

        playerR[n].info();
    }
}

int SaveSlot::getLevelNum()
{
    return levelNum;
}

int SaveSlot::getGameMode()
{
    return gameMode;
}
