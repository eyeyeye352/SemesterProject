#include "settings.h"

Settings::Settings() {}

int Settings::screenWidth = 700;
int Settings::screenHeight = 700;
QString Settings::gameIconPath = ":/icons/src/icon/gameIcon.ico";

double Settings::backgroundMoveSpeed = 1;
int Settings::backgroundUpdateInterval = 16;

int Settings::classicBtnY = 200;
int Settings::hexBtnY = 350;

double Settings::musicVol = 1;
double Settings::soundVol = 1;

double Settings::startBtnOriginScale = 2;
double Settings::funcBtnOriginScale = 1;
double Settings::funcBtnOriginOpacity = 1;

double Settings::levelBlockOriginScale = 1;
double Settings::levelBlockSpacing = 35;

double Settings::textBlockSize = 50;

double Settings::startSceneTitleScale = 1.3;
int Settings::startSceneTitleY = 50;
int Settings::levelSceneTiTleY = 600;

int Settings::functionBtnInterval = 5;

int Settings::loadingTextX = 10;

QPointF Settings::settingPage_MusicValSetPos = {150,170};
QPointF Settings::settingPage_SoundValSetPos = {150,280};

int Settings::sideBarWidth = 200;
int Settings::translateIconInterval = 20;

QPointF Settings::levelscene_stepTextPos = {10,10};

QPointF Settings::completeScene_stepTextPos = {20,150};
QPointF Settings::completeScene_scorePos = {20,250};
