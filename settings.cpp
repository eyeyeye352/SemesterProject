#include "settings.h"

Settings::Settings() {}

int Settings::screenWidth = 700;
int Settings::screenHeight = 700;

double Settings::backgroundMoveSpeed = 1;
int Settings::backgroundUpdateInterval = 16;

int Settings::classicBtnY = 200;
int Settings::hexBtnY = 350;

double Settings::musicVol = 1;
double Settings::soundVol = 1;

double Settings::startBtnOriginScale = 2;
double Settings::funcBtnOriginScale = 1;
double Settings::funcBtnOriginOpacity = 0.5;

double Settings::levelBlockOriginScale = 1;
double Settings::levelBlockSpacing = 35;

double Settings::textBlockSize = 50;

QPointF Settings::settingPage_MusicValSet = {150,170};
QPointF Settings::settingPage_SoundValSet = {150,280};
