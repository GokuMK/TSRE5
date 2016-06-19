#include "PropertiesAbstract.h"

PropertiesAbstract::PropertiesAbstract() : QWidget() {
    foreach (QObject *child, children()) {
        if (child->isWidgetType()) {
            child->installEventFilter(this);
        }
    }
}

PropertiesAbstract::~PropertiesAbstract() {
}

void PropertiesAbstract::showObj(WorldObj* obj){
}

void PropertiesAbstract::updateObj(WorldObj* obj){
}

bool PropertiesAbstract::support(WorldObj* obj){
    return false;
}
