/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SpeedPost.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include <math.h>
#include <QDebug>

void SpeedPost::set(QString sh, FileBuffer* data) {
    if (sh == ("name")) {
        this->name = ParserX::GetString(data);
        return;
    }
    if (sh == ("milepost_digit_tex")) {
        this->milepostDigitTex = ParserX::GetString(data);
        return;
    }
    if (sh == ("speed_digit_tex")) {
        this->speedDigitTex = ParserX::GetString(data);
        return;
    }
    if (sh == ("speed_text_size")) {
        this->speedTextSize[0] = ParserX::GetNumber(data);
        this->speedTextSize[1] = ParserX::GetNumber(data);
        this->speedTextSize[2] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("milepost_text_size")) {
        this->milepostTextSize[0] = ParserX::GetNumber(data);
        this->milepostTextSize[1] = ParserX::GetNumber(data);
        this->milepostTextSize[2] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("speed_resume_sign_shape")) {
        this->speedResumeSignShapeName = ParserX::GetString(data);
        bool ok = false;
        this->speedResumeSignShapeCount = ParserX::GetNumberInside(data, &ok);
        if(!ok){
            this->speedResumeSignShape = 0;
            return;
        }
        this->speedResumeSignShape = new float[this->speedResumeSignShapeCount * 4];
        for (int i = 0; i<this->speedResumeSignShapeCount; i++) {
            this->speedResumeSignShape[i * 4 + 0] = ParserX::GetNumber(data);
            this->speedResumeSignShape[i * 4 + 1] = ParserX::GetNumber(data);
            this->speedResumeSignShape[i * 4 + 2] = ParserX::GetNumber(data);
            this->speedResumeSignShape[i * 4 + 3] = ParserX::GetNumber(data)*M_PI/180.0;
        }
        return;
        return;
    }
    if (sh == ("speed_warning_sign_shape")) {
        this->speedWarningSignShapeName = ParserX::GetString(data);
        bool ok = false;
        this->speedWarningSignShapeCount = ParserX::GetNumberInside(data, &ok);
        if(!ok){
            this->speedWarningSignShape = 0;
            return;
        }
        this->speedWarningSignShape = new float[this->speedWarningSignShapeCount * 4];
        for (int i = 0; i<this->speedWarningSignShapeCount; i++) {
            this->speedWarningSignShape[i * 4 + 0] = ParserX::GetNumber(data);
            this->speedWarningSignShape[i * 4 + 1] = ParserX::GetNumber(data);
            this->speedWarningSignShape[i * 4 + 2] = ParserX::GetNumber(data);
            this->speedWarningSignShape[i * 4 + 3] = ParserX::GetNumber(data)*M_PI/180.0;
        }
        return;
        return;
    }
    if (sh == ("speed_sign_shape")) {
        this->speedSignShapeName = ParserX::GetString(data);
        bool ok = false;
        this->speedSignShapeCount = ParserX::GetNumberInside(data, &ok);
        if(!ok){
            this->speedSignShape = 0;
            return;
        }
        this->speedSignShape = new float[this->speedSignShapeCount * 4];
        for (int i = 0; i<this->speedSignShapeCount; i++) {
            this->speedSignShape[i * 4 + 0] = ParserX::GetNumber(data);
            this->speedSignShape[i * 4 + 1] = ParserX::GetNumber(data);
            this->speedSignShape[i * 4 + 2] = ParserX::GetNumber(data);
            this->speedSignShape[i * 4 + 3] = ParserX::GetNumber(data)*M_PI/180.0;
        }
        return;
    }
    if (sh == ("milepost_shape")) {
        this->milepostShapeName = ParserX::GetString(data);
        bool ok = false;
        this->milepostShapeCount = ParserX::GetNumberInside(data, &ok);
        if(!ok){
            this->milepostShapeCount = 0;
            return;
        }
        this->milepostShape = new float[this->milepostShapeCount * 4];
        for (int i = 0; i<this->milepostShapeCount; i++) {
            this->milepostShape[i * 4 + 0] = ParserX::GetNumber(data);
            this->milepostShape[i * 4 + 1] = ParserX::GetNumber(data);
            this->milepostShape[i * 4 + 2] = ParserX::GetNumber(data);
            this->milepostShape[i * 4 + 3] = ParserX::GetNumber(data)*M_PI/180.0;
        }
        return;
    }
    qDebug() << "#speedpost_set - undefined token: " << sh;
    return;
}

SpeedPost::SpeedPost() {
}

SpeedPost::~SpeedPost() {
}

