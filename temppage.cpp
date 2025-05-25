#include "temppage.h"
#include "myalgorithms.h"


TempPage::TempPage(QObject *parent):
    Scene(parent)
{}

TempPageBtn* TempPage::addBtn(QString text,QGraphicsItem* btnParent)
{
    TempPageBtn* btn = new TempPageBtn(btnParent);
    btn->setText(text);
    btns.append(btn);

    //默认将btn放在父item的右侧，从上而下
    if(btnParent != nullptr){
        btn->setPos(btnParent->boundingRect().right(), 10 + 60*(btns.size()-1));
    }else{
        btn->setPos(0, 10 + 60*(btns.size()-1));
        addItem(btn);
    }

    return btn;
}




SettingPage::SettingPage(QObject *parent)
    :TempPage{parent}
{

    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/settingPage.png"));
    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);
    addItem(bg);

    //Valsets
    musicSli = new ValSets(bg);
    soundSli = new ValSets(bg);
    musicSli->setPos(Settings::settingPage_MusicValSetPos);
    soundSli->setPos(Settings::settingPage_SoundValSetPos);

    //设置页面内部连接信号更改声音
    connect(musicSli,&ValSets::valueChanged,[this](double vol){
        Settings::musicVol = vol;
        MusicPlayer::getMPlayer()->setBgmVol(vol);
    });
    connect(soundSli,&ValSets::valueChanged,[this](double vol){
        Settings::soundVol = vol;
        MusicPlayer::getMPlayer()->setSoundVol(vol);
    });


    //btn
    backBtn = addBtn("Back",bg);
    homeBtn = addBtn("Home",bg);

    QObject::connect(backBtn,&GameBtn::clicked,[this]{ emit closePage();});
    QObject::connect(homeBtn,&GameBtn::clicked,[this]{ emit goHome(); });

}


TipPage::TipPage(QObject *parent):
    TempPage(parent)
{

    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/tipPage.png"));
    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);
    addItem(bg);

    answer = new QGraphicsTextItem(bg);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/chineseMonoSpace.ttf",answer,Qt::white,16);

    //btn
    backBtn = addBtn("Back",bg);
    QObject::connect(backBtn,&GameBtn::clicked,[this]{ emit closePage(); });

}

void TipPage::setHexAnswer(QString text, int radius)
{
    //wordNum表示一行的字数
    int wordNum = radius;
    int spaceNum = radius-1;
    QString ansText;

    for (int line = 1; line <= 2*radius-1; ++line) {

        ansText.append(QString(spaceNum,' ')+text.left(wordNum)+'\n');
        text.remove(0,wordNum);

        //前半部分，每行使wordNum+2
        if(line < radius){
            ++wordNum;
            --spaceNum;
        }
        //后半部分每行-2
        else{
            --wordNum;
            ++spaceNum;
        }
    }

    answer->setPlainText(ansText);
    answer->setPos((bg->boundingRect().width() - answer->boundingRect().width())/2,
                   100);
}

void TipPage::resetAnswer()
{
    answer->setPlainText("");
}

void TipPage::setClassicAnswer(QString text,int cols)
{
    //text传入全文内容，此处根据cols数量进行换行处理。
    QString ansText;
    for (int n = 0; n < text.size(); ++n) {
        if(n % cols == 0 && n != 0){
            ansText.append('\n');
        }
        ansText.append(text[n]);
    }

    qDebug() << "anstext:" << ansText;
    answer->setPlainText(ansText);
    answer->setPos((bg->boundingRect().width() - answer->boundingRect().width())/2,
                   100);

}





CompletePage::CompletePage(QObject *parent):
    TempPage(parent)
{

    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/completePage.png"));
    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);
    addItem(bg);

    stepText = new QGraphicsTextItem(bg);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",stepText,Qt::white,22);
    stepText->setPos(Settings::completeScene_stepTextPos);

    homeBtn = addBtn("Home",bg);
    connect(homeBtn,&GameBtn::clicked,[this]{ emit goHome();  });


}

void CompletePage::setContent(int step,QString time_spending)
{
    QString text = QString("Using step:        %1\n").arg(QString::number(step));
    text.append("time using:   " + time_spending);
    stepText->setPlainText(text);
}





SavePage::SavePage(QObject *parent):
    TempPage(parent),
    state(NONE)
{

    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/savePage.png"));
    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);
    addItem(bg);

    //设置bg为父item会出错，原因未知。
    SaveSlot* s1 = new SaveSlot;
    SaveSlot* s2 = new SaveSlot;
    SaveSlot* s3 = new SaveSlot;
    SaveSlot* s4 = new SaveSlot;

    s1->setPos(Settings::screenWidth/2 - s1->boundingRect().width() - 15,
               Settings::screenHeight/2 - s1->boundingRect().height() - 5);

    s2->setPos(Settings::screenWidth/2 + 15,
               Settings::screenHeight/2 - s2->boundingRect().height() - 5);

    s3->setPos(Settings::screenWidth/2 - s3->boundingRect().width() - 15,
               Settings::screenHeight/2 + 5);

    s4->setPos(Settings::screenWidth/2 + 15,
               Settings::screenHeight/2 + 5);

    SLslots.append(s1);
    SLslots.append(s2);
    SLslots.append(s3);
    SLslots.append(s4);

    for (int n = 0; n < SLslots.size(); ++n) {
        //根据state发出不同的信号
        connect(SLslots[n],&SaveSlot::clicked,[this,n]{
            emit slotSelected(n,state);
        });
        addItem(SLslots[n]);
    }


    back = addBtn("Back",bg);
    load = addBtn("Load",bg);
    save = addBtn("Save",bg);

    //back需要搭配黑幕，黑幕和back一起作为独立item
    back->setParentItem(nullptr);
    back->setPos(save->sceneBoundingRect().x(),bg->sceneBoundingRect().y() + 10);

    //save默认隐藏，处于levelscene时才由gamesys控制显示。
    save->hide();

    connect(back,&GameBtn::clicked,[this]{
        if(state == NONE){
            emit closePage();
        }else{
            backToNoneState();
        }
    });
    connect(save,&GameBtn::clicked,[this]{
        addBlack(SAVE);
    });
    connect(load,&GameBtn::clicked,[this]{
        addBlack(LOAD);
    });

    blackLay = new QGraphicsRectItem(0,0,Settings::screenWidth,Settings::screenHeight);
    blackLay->setBrush(Qt::black);
    addItem(blackLay);

    //back和slots的z层在black之上
    back->setZValue(blackLay->zValue()+1);
    for (int n = 0; n < SLslots.size(); ++n) {
        SLslots[n]->setZValue(blackLay->zValue()+1);
    }


    blackLay->setOpacity(0.7);
    blackLay->hide();

}

void SavePage::backToNoneState()
{
    state = NONE;
    blackLay->hide();
}

void SavePage::addBlack(int newState)
{
    state = newState;
    blackLay->show();
}

void SavePage::hideSaveBtn()
{
    save->hide();
}

void SavePage::showSaveBtn()
{
    save->show();
}


QList<SaveSlot*>& SavePage::getSlots()
{
    return SLslots;
}






RankPage::RankPage(QObject *parent):
    TempPage(parent)
{
    bg = new QGraphicsPixmapItem(QPixmap(":/background/src/background/rankPage.png"));
    bg->setPos((Settings::screenWidth - bg->boundingRect().width())/2,
               (Settings::screenHeight - bg->boundingRect().height())/2);
    addItem(bg);

    backBtn = addBtn("back",bg);
    connect(backBtn,&GameBtn::clicked,[this]{
        emit closePage();
    });

    timeOrderBtn = addBtn("Time-Order",bg);
    connect(timeOrderBtn,&GameBtn::clicked,[this]{ emit orderByTime(); });
    stepOrderBtn = addBtn("Step-Order",bg);
    connect(stepOrderBtn,&GameBtn::clicked,[this]{ emit orderByStep(); });

    description = new QGraphicsTextItem("No." + QString(7,' ') + "Date" +
                                            QString(8,' ') + "step" +
                                            QString(8,' ') + "use-time\n\n",bg);
    MyAlgorithms::addFontToTextItem(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",description,
                                    Qt::black,15);
    description->setPos((bg->boundingRect().width() - description->boundingRect().width())/2 - 30,100);

    text = new QTextEdit;
    text->setFont(MyAlgorithms::getFontByPath(":/fonts/src/fonts/AaHuanMengKongJianXiangSuTi-2.ttf",15));
    text->setTextColor(Qt::black);
    text->setFrameStyle(QFrame::NoFrame);
    text->setTextBackgroundColor(Qt::transparent);
    text->setFixedSize(400,250);
    text->setReadOnly(true);
    text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    text->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    text->setFocusPolicy(Qt::NoFocus);

    addWidget(text);
}

void RankPage::showContents(QList<RankRecord> &rList)
{

    QString content;

    for (int n = 0; n < rList.size(); ++n) {

        QString savedate = rList[n].save_date.toString("yyyy/MM/dd");
        int step = rList[n].step_using;
        QString timeuse = rList[n].time_spending.toString("HH:mm:ss");

        //十位数会多一格,最后一行就不加空行了。
        content.append(QString::number(n+1) + '.' + QString( (n+1<10?6:5),' ') +
                       savedate + QString(6,' ') + QString::number(step) +
                       QString(8,' ') + timeuse);
        if(n != rList.size()-1){
            content.append('\n');
        }

    }
    text->setPlainText(content);
    text->move((Settings::screenWidth - text->width())/2,
               description->sceneBoundingRect().y() + 50);
}
