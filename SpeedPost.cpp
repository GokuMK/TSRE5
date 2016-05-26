#include "SpeedPost.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include <QDebug>

void SpeedPost::set(QString sh, FileBuffer* data) {
    if (sh == ("name")) {
        this->name = ParserX::odczytajtc(data);
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
    if (sh == ("speed_resume_sign_shape")) {
        this->speedResumeSignShapeName = ParserX::odczytajtc(data);
        bool ok = false;
        this->speedResumeSignShapeCount = ParserX::parsujrInside(data, &ok);
        if(!ok){
            this->speedResumeSignShape = 0;
            return;
        }
        this->speedResumeSignShape = new float[this->speedResumeSignShapeCount * 4];
        for (int i = 0; i<this->speedResumeSignShapeCount; i++) {
            this->speedResumeSignShape[i * 4 + 0] = ParserX::parsujr(data);
            this->speedResumeSignShape[i * 4 + 1] = ParserX::parsujr(data);
            this->speedResumeSignShape[i * 4 + 2] = ParserX::parsujr(data);
            this->speedResumeSignShape[i * 4 + 3] = ParserX::parsujr(data);
        }
        return;
        return;
    }
    if (sh == ("speed_warning_sign_shape")) {
        this->speedWarningSignShapeName = ParserX::odczytajtc(data);
        bool ok = false;
        this->speedWarningSignShapeCount = ParserX::parsujrInside(data, &ok);
        if(!ok){
            this->speedWarningSignShape = 0;
            return;
        }
        this->speedWarningSignShape = new float[this->speedWarningSignShapeCount * 4];
        for (int i = 0; i<this->speedWarningSignShapeCount; i++) {
            this->speedWarningSignShape[i * 4 + 0] = ParserX::parsujr(data);
            this->speedWarningSignShape[i * 4 + 1] = ParserX::parsujr(data);
            this->speedWarningSignShape[i * 4 + 2] = ParserX::parsujr(data);
            this->speedWarningSignShape[i * 4 + 3] = ParserX::parsujr(data);
        }
        return;
        return;
    }
    if (sh == ("speed_sign_shape")) {
        this->speedSignShapeName = ParserX::odczytajtc(data);
        bool ok = false;
        this->speedSignShapeCount = ParserX::parsujrInside(data, &ok);
        if(!ok){
            this->speedSignShape = 0;
            return;
        }
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
        bool ok = false;
        this->milepostShapeCount = ParserX::parsujrInside(data, &ok);
        if(!ok){
            this->milepostShapeCount = 0;
            return;
        }
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

