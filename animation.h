#ifndef ANIMATION_H
#define ANIMATION_H

#include "scene.h"

class Animation
{
    Animation();
public:

    static void goLevelSelection(StartScene*);
    static void backModeSelection(StartScene*);

    static void changeMusic(QUrl);
    static void changeScene(Scene * origin,Scene * dest,QGraphicsView* view,int loadingTime);


    static QPropertyAnimation* MakeAnime(QObject* obj,QByteArray property,int time,QVariant sval,QVariant eval);

};



#endif // ANIMATION_H
