#include "SpeedPost.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include <QDebug>

void SpeedPost::set(QString sh, FileBuffer* data) {
    if (sh == ("name")) {
        this->name = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("speed_resume_sign_shape")) {
        this->speedResumeSignShape = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("milepost_digit_tex")) {
        this->milepostDigitTex = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("speed_digit_tex")) {
        this->speedDigitTex = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("speed_text_size")) {
        this->speedTextSize[0] = ParserX::parsujr(data);
        this->speedTextSize[1] = ParserX::parsujr(data);
        this->speedTextSize[2] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("milepost_text_size")) {
        this->milepostTextSize[0] = ParserX::parsujr(data);
        this->milepostTextSize[1] = ParserX::parsujr(data);
        this->milepostTextSize[2] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("speed_sign_shape")) {
        this->speedSignShapeName = ParserX::odczytajtc(data);
        this->speedSignShapeCount = ParserX::parsujr(data);
        this->speedSignShape = new float[this->speedSignShapeCount * 4];
        for (int i = 0; i<this->speedSignShapeCount; i++) {
            this->speedSignShape[i * 4 + 0] = ParserX::parsujr(data);
            this->speedSignShape[i * 4 + 1] = ParserX::parsujr(data);
            this->speedSignShape[i * 4 + 2] = ParserX::parsujr(data);
            this->speedSignShape[i * 4 + 3] = ParserX::parsujr(data);
        }
        return;
    }
    if (sh == ("milepost_shape")) {
        this->milepostShapeName = ParserX::odczytajtc(data);
        this->milepostShapeCount = ParserX::parsujr(data);
        this->milepostShape = new float[this->milepostShapeCount * 4];
        for (int i = 0; i<this->milepostShapeCount; i++) {
            this->milepostShape[i * 4 + 0] = ParserX::parsujr(data);
            this->milepostShape[i * 4 + 1] = ParserX::parsujr(data);
            this->milepostShape[i * 4 + 2] = ParserX::parsujr(data);
            this->milepostShape[i * 4 + 3] = ParserX::parsujr(data);
        }
        return;
    }/**/
    qDebug() << "speedpost value unknown: " << sh;
    return;
}

SpeedPost::SpeedPost() {
}

SpeedPost::~SpeedPost() {
}

