#include "PropertiesAbstract.h"

PropertiesAbstract::PropertiesAbstract() : QWidget() {
}

PropertiesAbstract::~PropertiesAbstract() {
}

void PropertiesAbstract::showObj(WorldObj* obj){
}

bool PropertiesAbstract::support(WorldObj* obj){
    return false;
}