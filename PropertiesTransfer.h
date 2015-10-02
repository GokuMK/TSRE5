#ifndef PROPERTIESTRANSFER_H
#define	PROPERTIESTRANSFER_H

#include "PropertiesAbstract.h"

class PropertiesTransfer : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesTransfer();
    virtual ~PropertiesTransfer();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
private:
    QPixmap* texPreview;
    QLabel* texPreviewLabel;
    QLineEdit sizeX;
    QLineEdit sizeY;
};

#endif	/* PROPERTIESTRANSFER_H */

