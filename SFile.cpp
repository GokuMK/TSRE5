/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SFile.h"
#include "SFileC.h"
#include "SFileX.h"
#include "ReadFile.h"
#include "ParserX.h"
#include "TexLib.h"
#include <QDebug>
#include <QtCore>
#include <iostream>
#include <QOpenGLShaderProgram>
#include "GLUU.h"
#include "GLMatrix.h"
#include <QString>
#include "Game.h"
#include "TS.h"
#include "ShapeTextureInfo.h"
#include "ShapeHierarchyInfo.h"
#include "ContentHierarchyInfo.h"

SFile::SFile() {
    pathid = "";
    this->loaded = 2;
}

SFile::SFile(QString pathid, QString name, QString texp ) {
    this->pathid = pathid;
    this->nazwa = name;
    this->isinit = 1;
    this->loaded = 0;
    this->texPath = texp;
    state.push_back(State());
}

SFile::SFile(const SFile& orig) {
}

SFile::~SFile() {
}

void SFile::load() {
    //unsigned long long int timeNow = QDateTime::currentMSecsSinceEpoch();
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << "S Shape: not exist "<<pathid;
        file->close();
        return;
    }
    FileBuffer* data = ReadFile::read(file);
    int loadingCount = 0;
    //qDebug() << "--" << pathid << "--" << data->length;

    data->off = 32;
    if (data->getInt() == 71) {
        int pozycja, offset, akto;
        data->off = 32;
        int val = data->getInt();
        
        //qDebug() << val << " plik binarny ";
        //wczytanie binarnego
        data->off += 5;
        for (;;) {
            pozycja = data->getInt();
            offset = data->getInt();
            akto = data->off;
            //qDebug() << "#SFile - token: "<< pozycja << TS::IdName[pozycja];
            switch (pozycja) {
                case 70:
                    break;
                case 68:
                    break;
                case 72:
                    loadingCount++;
                    SFileC::odczytajshaders(data, this);
                    break;
                case 7:
                    loadingCount++;
                    SFileC::odczytajpunktyc(data, this);
                    getSize();
                    break;
                case 9:
                    loadingCount++;
                    SFileC::odczytajuvpunktyc(data, this);
                    break;
                case 5:
                    loadingCount++;
                    SFileC::odczytajnormalnec(data, this);
                    break;
                case 66:
                    loadingCount++;
                    SFileC::odczytajmatricesc(data, this);
                    break;
                case 14:
                    loadingCount++;
                    SFileC::odczytajimagesc(data, this);
                    break;
                case 16:
                    loadingCount++;
                    SFileC::odczytajtexturesc(data, this);
                    break;
                case 47:
                    loadingCount++;
                    SFileC::odczytajvtx_statesc(data, this);
                    break;
                case 55:
                    loadingCount++;
                    SFileC::odczytajprim_statesc(data, this);
                    break;
                case 74:
                    break;
                case 76:
                    break;
                case 11:
                    break;
                case 18:
                    break;
                case 79:
                    break;
                case 31:
                    if(loadingCount < 9){
                        qDebug() << "#shape - loading error" << 31 << TS::IdName[31];
                        return;
                    }
                    SFileC::odczytajloddc(data, this);
                    loaded = 1;
                    break;
                case 29:
                    int pozycja1,offset1,akto1,some_val;
                    //some_val = data->getInt();
                    data->off+=5; // maybe read animation number instead?
                    //qDebug() << some_val;
                    //qDebug() << data->off << akto + offset;
                    //some_val = data->off+=4;
                    if(data->off >= akto + offset) break;
                    for (;;) {
                        pozycja1 = data->getInt();
                        offset1 = data->getInt();
                        akto1 = data->off;
                        switch (pozycja1) {
                            case 28:
                                animations.push_back(Animation());
                                animations.back().loadC(data, akto1 + offset1);
                                if(animations.size() > 0){
                                    //qDebug() << animations[0].node.size();
                                }
                                break;
                            default:
                                qDebug() << "#SFile Animations - unknown token: "<< pozycja1 << TS::IdName[pozycja1];
                                break;
                        }
                        data->off = akto1 + offset1;
                        if(data->off >= akto + offset) break;
                    }
                    break;
                default:
                    qDebug() << "#SFile - unknown token: "<< pozycja << TS::IdName[pozycja];
                    break;
            }
            data->off = akto + offset;
            if(data->off >= data->length) break;
        }
    } else {
        //qDebug() << "plik xml:";
        //wczytanie plku xml
        data->off = 0;
        ParserX::NextLine(data);

        QString sh = "";
        while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
            if(sh == "shape"){
                while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                    //qDebug() << sh;
                    if(sh == "shader_names"){
                        SFileX::odczytajshaders(data, this);
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "points"){
                        SFileX::odczytajpunkty(data, this);
                        getSize();
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "uv_points"){
                        SFileX::odczytajuvpunkty(data, this);
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "normals"){
                        SFileX::odczytajnormalne(data, this);
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "matrices"){
                        SFileX::odczytajmatrices(data, this);
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "images"){
                        SFileX::odczytajimages(data, this);
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "textures"){
                        SFileX::odczytajtextures(data, this);
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "vtx_states"){
                        SFileX::odczytajvtx_states(data, this);
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "prim_states"){
                        SFileX::odczytajprim_states(data, this);
                        loadingCount++;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "shape_header"){
                        //qDebug() << "shape_header" << 
                        ParserX::GetAlternativeTokenName(data);
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "volumes"){
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "texture_filter_names"){
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "sort_vectors"){
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "colours"){
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "light_materials"){
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "light_model_cfgs"){
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "lod_controls"){
                        if(loadingCount < 9){
                            qDebug() << "#shape - loading error" << sh;
                            return;
                        }
                        SFileX::odczytajlodd(data, this);
                        loaded = 1;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    if(sh == "animations"){
                        while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                            if(sh == "animation"){
                                animations.push_back(Animation());
                                animations.back().loadX(data);
                                if(animations.size() > 0){
                                    qDebug() << animations[0].node.size();
                                    /*for(int i = 0; i < animations[0].node.size(); i++){
                                        qDebug() << animations[0].node[i].linearKey.size();
                                        qDebug() << animations[0].node[i].slerpRot.size();
                                        qDebug() << animations[0].node[i].tcbKey.size();
                                    }*/
                                }
                                ParserX::SkipToken(data);
                                continue;
                            }
                            ParserX::SkipToken(data);
                        }
                        ParserX::SkipToken(data);
                        continue;
                    }
                    qDebug() << "#shape - undefined token" << sh;
                    ParserX::SkipToken(data);
                }
                
                ParserX::SkipToken(data);
                continue;
            }
            qDebug() << "#SFile - undefined token" << sh;
            ParserX::SkipToken(data);
        }
    }
    delete data;
    file->close();
    if(loaded == 1)
        buildFrameIds();
    loadSd();
    //qDebug() <<this->pathid << QDateTime::currentMSecsSinceEpoch() - timeNow;
    return;
}

void SFile::Animation::loadC(FileBuffer* data, int length){
    data->off++;
    frames = data->getInt();
    fps = data->getInt();
    int pozycja, offset, akto;
    int pozycja1, offset1, akto1;
    int pozycja2, offset2, akto2;
    int nodeCount, cCount, kCount;
    
    for (;;) {
        pozycja = data->getInt();
        offset = data->getInt();
        akto = data->off;

        switch (pozycja) {
            case 27:
                data->off++;
                nodeCount = data->getInt();
                for(int i = 0; i < nodeCount; i++){
                    node.push_back(AnimNode());
                    pozycja1 = data->getInt();
                    offset1 = data->getInt();
                    akto1 = data->off;
                    int temp = data->get();
                    data->off += temp*2;
                    data->off++;
                    data->off += 8;
                    cCount = data->getInt();
                    //qDebug() << cCount;
                    for(int j = 0; j < cCount; j++){
                        pozycja2 = data->getInt();
                        offset2 = data->getInt();
                        akto2 = data->off;
                        switch(pozycja2){
                            case 24:
                                data->off++;
                                kCount = data->getInt();
                                //qDebug() << "kCount"<<kCount;
                                for(int ii = 0; ii < kCount; ii++){
                                    int tcbid = data->getInt();
                                    data->getInt();
                                    data->off++;
                                    if(tcbid == 20){
                                        node.back().tcbKey.push_back(AnimNode::TcbKey());
                                        node.back().tcbKey.back().frame = data->getUint();
                                        node.back().tcbKey.back().quat[0] = -data->getFloat();
                                        node.back().tcbKey.back().quat[1] = data->getFloat();
                                        node.back().tcbKey.back().quat[2] = -data->getFloat();
                                        node.back().tcbKey.back().quat[3] = data->getFloat();
                                        for(int i = 0; i < 5; i++){
                                            node.back().tcbKey.back().param[i] = data->getFloat();
                                        }
                                    }
                                    if(tcbid == 23){
                                        node.back().slerpRot.push_back(AnimNode::SlerpRot());
                                        node.back().slerpRot.back().frame = data->getUint();
                                        node.back().slerpRot.back().quat[0] = -data->getFloat();
                                        node.back().slerpRot.back().quat[1] = data->getFloat();
                                        node.back().slerpRot.back().quat[2] = -data->getFloat();
                                        node.back().slerpRot.back().quat[3] = data->getFloat();
                                    }
                                }
                                break;
                            case 21:
                                data->off++;
                                kCount = data->getInt();
                                //qDebug() << "kCount"<<kCount;
                                for(int ii = 0; ii < kCount; ii++){
                                    //qDebug() << 
                                    data->getInt();
                                    data->getInt();
                                    data->off++;
                                    node.back().linearKey.push_back(AnimNode::LinearKey());
                                    node.back().linearKey.back().frame = data->getUint();
                                    for(int i = 0; i < 3; i++){
                                        node.back().linearKey.back().pos[i] = data->getFloat();
                                    }
                                }
                                break;
                            default:
                                qDebug() << "#controller" << pozycja2 << TS::IdName[pozycja2];
                                break;
                        }
                        data->off = akto2 + offset2;
                        if(data->off >= akto1+offset1) break;
                    }
                    //qDebug() << "pozycja" << pozycja1 << TS::IdName[pozycja1];
                    data->off = akto1+offset1;
                }
                break;
            default:
                qDebug() << "#SFile Animation - unknown token: "<< pozycja << TS::IdName[pozycja];
                break;
        }
        data->off = akto + offset;
        if(data->off >= length) break;
    }
}

void SFile::Animation::loadX(FileBuffer* data){
    frames = ParserX::GetNumber(data);
    fps = ParserX::GetNumber(data);
    
    QString sh;
    int count;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if(sh == "anim_nodes"){
            count = ParserX::GetNumber(data);
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if(sh == "anim_node"){
                    //qDebug() << 
                    ParserX::NextTokenInside(data);
                    node.push_back(AnimNode());
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if(sh == "controllers"){
                            count = ParserX::GetNumber(data);
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if(sh == "tcb_rot"){
                                    count =  ParserX::GetNumber(data);
                                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                        if(sh == "tcb_key"){
                                            node.back().tcbKey.push_back(AnimNode::TcbKey());
                                            node.back().tcbKey.back().frame = ParserX::GetUInt(data);
                                            node.back().tcbKey.back().quat[0] = -ParserX::GetNumber(data);
                                            node.back().tcbKey.back().quat[1] = ParserX::GetNumber(data);
                                            node.back().tcbKey.back().quat[2] = -ParserX::GetNumber(data);
                                            node.back().tcbKey.back().quat[3] = ParserX::GetNumber(data);
                                            for(int i = 0; i < 5; i++){
                                                node.back().tcbKey.back().param[i] = ParserX::GetNumber(data);
                                            }
                                            ParserX::SkipToken(data);
                                            continue;
                                        }
                                        if(sh == "slerp_rot"){
                                            node.back().slerpRot.push_back(AnimNode::SlerpRot());
                                            node.back().slerpRot.back().frame = ParserX::GetUInt(data);
                                            node.back().slerpRot.back().quat[0] = -ParserX::GetNumber(data);
                                            node.back().slerpRot.back().quat[1] = ParserX::GetNumber(data);
                                            node.back().slerpRot.back().quat[2] = -ParserX::GetNumber(data);
                                            node.back().slerpRot.back().quat[3] = ParserX::GetNumber(data);
                                            ParserX::SkipToken(data);
                                            continue;
                                        }
                                        ParserX::SkipToken(data);
                                    }
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if(sh == "linear_pos"){
                                    count = ParserX::GetNumber(data);
                                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                        if(sh == "linear_key"){
                                            node.back().linearKey.push_back(AnimNode::LinearKey());
                                            node.back().linearKey.back().frame = ParserX::GetUInt(data);
                                            for(int i = 0; i < 3; i++){
                                                node.back().linearKey.back().pos[i] = ParserX::GetNumber(data);
                                            }
                                            ParserX::SkipToken(data);
                                            continue;
                                        }
                                        ParserX::SkipToken(data);
                                    }
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                ParserX::SkipToken(data);
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }
}

void SFile::loadSd() {
    if(loadedSd == true)
        return;
    QFile file(pathid+"d");
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "Sd Shape: not exist "<<pathid+"d";
        return;
    }
    FileBuffer* data = ReadFile::read(&file);
    file.close();
    data->toUtf16();
    data->skipBOM();
    QString sh;
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("simisa@@@@@@@@@@jinx0t1t______")) {
            continue;
        }
        if (sh == ("shape")) {
            sdName = ParserX::GetString(data).trimmed();
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("esd_detail_level")) {
                    esdDetailLevel = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("esd_alternative_texture")) {
                    esdAlternativeTexture = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("esd_bounding_box")) {
                    esdBoundingBox << EsdBoundingBox();
                    bool ok = false;
                    for(int i = 0; i < 6; i++){
                        esdBoundingBox.back().shape[i] = ParserX::GetNumberInside(data, &ok);
                        if(!ok) break;
                    }
                    if(ok == false)
                        esdBoundingBox.pop_back();
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("esd_complex")) {
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == ("esd_complex_box")) {
                            esdBoundingBox << EsdBoundingBox();
                            for(int i = 0; i < 3; i++){
                                esdBoundingBox.back().rotation[i] = ParserX::GetNumber(data);
                            }
                            for(int i = 0; i < 3; i++){
                                esdBoundingBox.back().translation[i] = ParserX::GetNumber(data);
                            }
                            for(int i = 0; i < 6; i++){
                                esdBoundingBox.back().shape[i] = ParserX::GetNumber(data);
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("esd_snapable")) {
                    snapable = true;
                    ParserX::SkipToken(data);
                    continue;
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
    }
    
    loadedSd = true;
    delete data;
}

void SFile::getSize() {
        float tx[2]{-9999, 9999}, ty[2]{-9999, 9999}, tz[2]{-9999, 9999};
        for (int i = 0; i < tpoints.ipoints ; i++) {
            if(tpoints.points[i].x < tx[1]) tx[1] = tpoints.points[i].x;
            if(tpoints.points[i].y < ty[1]) ty[1] = tpoints.points[i].y;
            if(tpoints.points[i].z < tz[1]) tz[1] = tpoints.points[i].z;
            if(tpoints.points[i].x > tx[0]) tx[0] = tpoints.points[i].x;
            if(tpoints.points[i].y > ty[0]) ty[0] = tpoints.points[i].y;
            if(tpoints.points[i].z > tz[0]) tz[0] = tpoints.points[i].z;
        }
        bound[0] = tx[0];
        bound[1] = tx[1];
        bound[2] = ty[0];
        bound[3] = ty[1];
        bound[4] = tz[0];
        bound[5] = tz[1];
        tx[0] = tx[0] - tx[1];
        ty[0] = ty[0] - ty[1];
        tz[0] = tz[0] - tz[1];
        size = sqrt(tx[0]*tx[0] + ty[0]*ty[0] + tz[0]*tz[0]);
        //console.log(this.size);
    }

bool SFile::getBoxPoints(QVector<float>& points){
    if(this->esdBoundingBox.size() == 0){
        for(int i=0; i<2; i++)
            for(int j=4; j<6; j++){
                points.push_back(-bound[i]);
                points.push_back(bound[2]);
                points.push_back(bound[j]);
                points.push_back(-bound[i]);
                points.push_back(bound[3]);
                points.push_back(bound[j]);
            }
        for(int i=0; i<2; i++)
            for(int j=2; j<4; j++){
                points.push_back(-bound[i]);
                points.push_back(bound[j]);
                points.push_back(bound[4]);
                points.push_back(-bound[i]);
                points.push_back(bound[j]);
                points.push_back(bound[5]);
            }
        for(int i=4; i<6; i++)
            for(int j=2; j<4; j++){
                points.push_back(-bound[0]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
                points.push_back(-bound[1]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
            }
    } else {
        float tbound[6];
        for(int u = 0; u < this->esdBoundingBox.size(); u++ ){
            tbound[0] = this->esdBoundingBox[u].shape[0];
            tbound[1] = this->esdBoundingBox[u].shape[3];
            tbound[2] = this->esdBoundingBox[u].shape[1];
            tbound[3] = this->esdBoundingBox[u].shape[4];
            tbound[4] = this->esdBoundingBox[u].shape[2];
            tbound[5] = this->esdBoundingBox[u].shape[5];
            
            //int pointsBeg = points.size();
            ///////////
            for(int i=0; i<2; i++)
                for(int j=4; j<6; j++){
                    points.push_back(-tbound[i]);
                    points.push_back(tbound[2]);
                    points.push_back(tbound[j]);
                    points.push_back(-tbound[i]);
                    points.push_back(tbound[3]);
                    points.push_back(tbound[j]);
                }
            for(int i=0; i<2; i++)
                for(int j=2; j<4; j++){
                    points.push_back(-tbound[i]);
                    points.push_back(tbound[j]);
                    points.push_back(tbound[4]);
                    points.push_back(-tbound[i]);
                    points.push_back(tbound[j]);
                    points.push_back(tbound[5]);
                }
            for(int i=4; i<6; i++)
                for(int j=2; j<4; j++){
                    points.push_back(-tbound[0]);
                    points.push_back(tbound[j]);
                    points.push_back(tbound[i]);
                    points.push_back(-tbound[1]);
                    points.push_back(tbound[j]);
                    points.push_back(tbound[i]);
                }
            //int pointsEnd = points.size();
            //for(int i = pointsBeg; i < pointsEnd; i++){
            //    //Vec3::add(&points[i], &points[i], this->esdBoundingBox[u].translation);
            //}
        }
    }
    return true;
}

void SFile::getFloorBorderLinePoints(float *&punkty){
    if(this->esdBoundingBox.size() == 0){
        float h = bound[2];
        if(bound[3] < bound[2])
            h = bound[3];
//0 1 / 4 5
        float vec1[3], vec2[3], dist;
        Vec3::set(vec1, bound[0], h, bound[4]);
        Vec3::set(vec2, bound[0], h, bound[5]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[0], h, bound[5]);
        Vec3::set(vec2, bound[1], h, bound[5]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[1], h, bound[4]);
        Vec3::set(vec2, bound[0], h, bound[4]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[1], h, bound[5]);
        Vec3::set(vec2, bound[1], h, bound[4]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
    } else {
        float tbound[6];
        for(int u = 0; u < this->esdBoundingBox.size(); u++ ){
            tbound[0] = this->esdBoundingBox[u].shape[0];
            tbound[1] = this->esdBoundingBox[u].shape[3];
            tbound[2] = this->esdBoundingBox[u].shape[1];
            tbound[3] = this->esdBoundingBox[u].shape[4];
            tbound[4] = this->esdBoundingBox[u].shape[2];
            tbound[5] = this->esdBoundingBox[u].shape[5];
            
        float h = tbound[2];
        if(tbound[3] < tbound[2])
            h = tbound[3];
        float vec1[3], vec2[3], dist;
        Vec3::set(vec1, bound[0], h, bound[4]);
        Vec3::set(vec2, bound[0], h, bound[5]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[0], h, bound[5]);
        Vec3::set(vec2, bound[1], h, bound[5]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[1], h, bound[4]);
        Vec3::set(vec2, bound[0], h, bound[4]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[1], h, bound[5]);
        Vec3::set(vec2, bound[1], h, bound[4]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        }
    }
}

bool SFile::isSnapable(){
    return snapable;
}

void SFile::addSnapablePoints(QVector<float> &out){
    //if(!this->loadedSd)
    //    loadSd();
    if(esdBoundingBox.size() == 0) return;
    float avgZ = (esdBoundingBox[0].shape[2] + esdBoundingBox[0].shape[5])/2.0;
    out.push_back(esdBoundingBox[0].shape[0]);
    out.push_back(0);
    out.push_back(avgZ);
    out.push_back(esdBoundingBox[0].shape[3]);
    out.push_back(0);
    out.push_back(avgZ);
}

void SFile::reload() {
    loaded = 0;
    qDebug() << "reload";
    QStringList list;
    
    for (int i = 0; i < distancelevel[0].iloscs; i++) {
        for (int j = 0; j < distancelevel[0].subobiekty[i].iloscc; j++) {
            int prim_state = distancelevel[0].subobiekty[i].czesci[j].prim_state_idx;

            if(primstate[prim_state].arg4 == -1)
                continue;
            //if (image[texture[primstate[prim_state].arg4].image].tex == -2)
            //    continue;
            list.push_back(image[texture[primstate[prim_state].arg4].image].name);
       }
    }
    
    list.removeDuplicates();
    for(QString s : list){
        qDebug() << s;
        TexLib::addTex(texPath, s, true);
    }
}

unsigned int SFile::newState(){
    state.push_back(State());
    return state.size() - 1;
}

void SFile::setAnimated(unsigned int stateId, bool animated){
    state[stateId].animated = animated;
}

void SFile::setEnabledSubObjs(unsigned int stateId, unsigned int enabledSubObjs){
    state[stateId].enabledSubObjs = enabledSubObjs;
}

void SFile::setCurrentDistanceLevel(unsigned int stateId, int level){
    if(level < 0)
        level = 0;
    if(level >= iloscd)
        level = 0;
    state[stateId].distanceLevel = level;
    qDebug() << state[stateId].distanceLevel;
}

void SFile::enableSubObjByNameQueue(unsigned int stateId, QString name, bool val){
    if(stateId > state.size() - 1) 
        return;
    state[stateId].enableSubObjQueue[name] = val;
}

void SFile::enableSubObjByName(unsigned int stateId, QString name, bool val){
    // Find matrix id
    int matrixId = -1;
    for(int i = 0; i < iloscm; i++) {
        if(macierz[i].name.toLower() == name.toLower() ) {
            matrixId = i;
            break;
        }
    }
    if(matrixId < 0)
        return;
    
    // check if can be disabled/enabled
    if(distancelevel[0].iloscs < 2)
        return;
    //qDebug() << matrixId << distancelevel[0].subobiekty[0].header.geometryNodeMap.size() ;
    if(matrixId >= distancelevel[0].subobiekty[0].header.geometryNodeMap.size())
        return;
    if(distancelevel[0].subobiekty[0].header.geometryNodeMap[matrixId] > 0 )
        return;
    
    // enable / disable subobjs
    for(int j = 1; j < distancelevel[0].iloscs; j++){
        if(distancelevel[0].subobiekty[j].header.geometryNodeMap[matrixId] == 0){
            if(val)
                state[stateId].enabledSubObjs = state[stateId].enabledSubObjs | (1 << j);
            else
                state[stateId].enabledSubObjs = state[stateId].enabledSubObjs & ~(1 << j);
        }
    }
}

void SFile::updateSim(float deltaTime, unsigned int stateId){
    if (isinit != 1 || loaded == 2)
        return;
    
    animated = false;

    if(state[stateId].animated && animations.size() > 0){
        if(animations[0].frames == 0)
            return;
        animated = true;
        if(deltaTime <= 0){
            state[stateId].frameCount = 0;
        } else {
            state[stateId].frameCount += animations[0].fps * deltaTime;
            if(state[stateId].frameCount > animations[0].frames)
                while(state[stateId].frameCount > animations[0].frames)
                    state[stateId].frameCount -= animations[0].frames;
            if(state[stateId].frameCount < 0)
                state[stateId].frameCount = 0;
        }
    }
}
void SFile::render() {
    render(0,0);
}

void SFile::render(int selectionColor, unsigned int stateId) {

    if (isinit != 1 || loaded == 2)
        return;
    if (loaded == 0) {
        if(Game::allowObjLag < 1)  return;
        
        Game::allowObjLag-=2;
        loaded = 2;
        load();
        return;
    }
    
    if(state[stateId].enableSubObjQueue.size() > 0){
        //qDebug() << "queue"<<state[stateId].enableSubObjQueue.size();
        for (auto it = state[stateId].enableSubObjQueue.begin(); it != state[stateId].enableSubObjQueue.end();){
            enableSubObjByName(stateId, it.key(), it.value());
            it = state[stateId].enableSubObjQueue.erase(it);
        }
    }

    int oldmatrix = -2;
    float m[16];
    //var tex;
    //var oldtex = -3;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    GLUU *gluu = GLUU::get();
    //gluu->enableTextures();
    //for(int iii = 0; iii < 200; iii++)
    //float talpha = gluu->alpha;
    //float talphatest = gluu->alphaTest;
    
    int currentDlevel = state[stateId].distanceLevel;
    for (int i = 0; i < distancelevel[currentDlevel].iloscs; i++) {
        QOpenGLVertexArrayObject::Binder vaoBinder(&distancelevel[currentDlevel].subobiekty[i].VAO);

        if(((state[stateId].enabledSubObjs >> i) & 1) == 0)
            continue;
        
        for (int j = 0; j < distancelevel[currentDlevel].subobiekty[i].iloscc; j++) {

            int prim_state = distancelevel[currentDlevel].subobiekty[i].czesci[j].prim_state_idx;
            int vtx_state = primstate[prim_state].vtx_state;
            int matrix = vtxstate[vtx_state].matrix;
            bool texEnabled = distancelevel[currentDlevel].subobiekty[i].czesci[j].enabled;

            if(animated){
                Mat4::identity(m);
                getPmatrixAnimated(currentDlevel, m, matrix, state[stateId].frameCount);
                oldmatrix = -1;
                gluu->currentMsMatrinxHash = 0;
                gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]>(&m));
            } else {
                if (oldmatrix != matrix) {
                    oldmatrix = matrix;
                    if (!macierz[matrix].isFixed) {
                        Mat4::identity(m);
                        memcpy(macierz[matrix].fixed, getPmatrix(currentDlevel, m, matrix), sizeof (float) * 16);
                        macierz[matrix].isFixed = true;
                        macierz[matrix].hash = gluu->getMatrixHash(macierz[matrix].fixed);
                    }
                    if(macierz[matrix].hash != gluu->currentMsMatrinxHash){
                        gluu->currentMsMatrinxHash = macierz[matrix].hash;
                        gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]>(&macierz[matrix].fixed));
                    }
                }
            }
            
            if( vtxstate[vtx_state].arg2 < -7 )
                gluu->disableNormals();
            else
                gluu->enableNormals();
            
            if( vtxstate[vtx_state].arg2 == -12 )
                gluu->setBrightness(0.5);
            else
                gluu->setBrightness(1.0);
            
            /*if(primstate[prim_state].arg2 < this->ishaders)
                gluu->alpha = shader[primstate[prim_state].arg2].alpha;
            else 
                gluu->alpha = 0;
            gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlpha, gluu->alpha);

            if(primstate[prim_state].arg6 == 1)
                gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlphaTest, 0.51f);
            else 
                gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlphaTest, gluu->alphaTest);
            */
            //if(gluu->textureEnabled)
            if(primstate[prim_state].arg4 == -1 || !texEnabled || TexLib::disabledTextures[image[texture[primstate[prim_state].arg4].image].texAddr] == 1){
                if(selectionColor == 0)
                    gluu->disableTextures(1.0, 0.0, 1.0, 1.0);
                //glDisable(GL_TEXTURE_2D);
            } else if (image[texture[primstate[prim_state].arg4].image].texAddr >= 0) {
                //glEnable(GL_TEXTURE_2D);
                gluu->bindTexture(f, image[texture[primstate[prim_state].arg4].image].texAddr);
                //f->glBindTexture(GL_TEXTURE_2D, image[texture[primstate[prim_state].arg4].image].texAddr);
            } else if (image[texture[primstate[prim_state].arg4].image].tex == -2) {
                //glDisable(GL_TEXTURE_2D);
            } else if (image[texture[primstate[prim_state].arg4].image].tex == -1) {
                //image[texture[primstate[prim_state].arg4].image].tex = -2;
                //qDebug() << this->nazwa;
                //if(this->nazwa.contains("pared1_I.s", Qt::CaseInsensitive)){
                //    qDebug() << vtxstate[vtx_state].arg2;
                //    qDebug() << "=========" << image[texture[primstate[prim_state].arg4].image].name;
                //}
                image[texture[primstate[prim_state].arg4].image].tex = TexLib::addTex(
                        texPath,
                        image[texture[primstate[prim_state].arg4].image].name
                        );
                //glDisable(GL_TEXTURE_2D);
            } else if (TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->glLoaded) {
                image[texture[primstate[prim_state].arg4].image].texAddr = TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->tex[0];
                //glDisable(GL_TEXTURE_2D);
            } else if (TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->loaded) {
                //if(allowLag) {
                //    allowLag = false;
                //if(TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->missing || TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->error){
                //    image[texture[primstate[prim_state].arg4].image].tex = -2;
                //} else {
                TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->GLTextures();
                //}
                //glDisable(GL_TEXTURE_2D);
                //}
            } else {
                //glDisable(GL_TEXTURE_2D);
            }/**/
            
            //QOpenGLVertexArrayObject::Binder vaoBinder(&distancelevel[0].subobiekty[i].czesci[j].VAO);
            f->glDrawArrays(GL_TRIANGLES, distancelevel[currentDlevel].subobiekty[i].czesci[j].offset, distancelevel[currentDlevel].subobiekty[i].czesci[j].iloscv);/**/
            
            if(selectionColor == 0)
                gluu->enableTextures();
        }
    }
    //gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlphaTest, gluu->alphaTest);
    gluu->setBrightness(1.0);
}

void SFile::fillContentHierarchyInfo(QVector<ContentHierarchyInfo*>& list, int parent){
    if (isinit != 1 || loaded != 1)
        return;
    
    ContentHierarchyInfo *info = new ContentHierarchyInfo();
    info->parent = parent;
    info->name = nazwa;
    info->distanceLevelId = -1; // Default
    info->sfile = this;
    info->type = "shape";
    list.push_back(info);
    parent = list.size()-1;
    
    for (int i = 0; i < iloscd; i++) {
        ContentHierarchyInfo *info = new ContentHierarchyInfo();
        info->parent = parent;
        info->name = "Distance Level: " + QString::number(distancelevel[i].levelSelection) + " m";
        info->distanceLevelId = i;
        info->sfile = this;
        info->type = "shape";
        list.push_back(info);
    }
}

void SFile::enablePart(unsigned int uid, unsigned int stateId){
    if (isinit != 1 || loaded != 1)
        return;
    
    int currentDlevel = state[stateId].distanceLevel;

    unsigned int i = uid / 1000;
    unsigned int j = uid - i*1000;
    distancelevel[currentDlevel].subobiekty[i].czesci[j].enabled = true;
}

void SFile::disablePart(unsigned int uid, unsigned int stateId){
    if (isinit != 1 || loaded != 1)
        return;
    
    int currentDlevel = state[stateId].distanceLevel;

    unsigned int i = uid / 1000;
    unsigned int j = uid - i*1000;
    distancelevel[currentDlevel].subobiekty[i].czesci[j].enabled = false;
}

void SFile::fillShapeHierarchyInfo(ShapeHierarchyInfo* info, unsigned int stateId){
    if(info == NULL)
        return;
    
    if (isinit != 1 || loaded != 1)
        return;
    
    int currentDlevel = state[stateId].distanceLevel;
    
    for(int i = 0; i < distancelevel[currentDlevel].ilosch; i++){
        info->hierarchy.push_back(distancelevel[currentDlevel].hierarchia[i]);
    }

    for(int i = 0; i < iloscm; i++){
        info->matrices.push_back(macierz[i].name);
    }

    for (int i = 0; i < distancelevel[currentDlevel].iloscs; i++) {
        for (int j = 0; j < distancelevel[currentDlevel].subobiekty[i].iloscc; j++) {
            int prim_state = distancelevel[currentDlevel].subobiekty[i].czesci[j].prim_state_idx;
            int vtx_state = primstate[prim_state].vtx_state;
            int matrix = vtxstate[vtx_state].matrix;
            
            info->parts.push_back(ShapeHierarchyInfo::ShapePart());
            info->parts.last().matrixId = matrix;
            if(primstate[prim_state].arg4 >= 0)
                info->parts.last().textureName = image[texture[primstate[prim_state].arg4].image].name;
            info->parts.last().polyCount = distancelevel[currentDlevel].subobiekty[i].czesci[j].iloscv/3;
            info->parts.last().uid = i*1000 + j;
            info->parts.last().enabled = distancelevel[currentDlevel].subobiekty[i].czesci[j].enabled;
        }
    }
}

void SFile::fillShapeTextureInfo(QHash<int, ShapeTextureInfo*>& list, unsigned int stateId){
    if (isinit != 1 || loaded != 1)
        return;
    
    int currentDlevel = state[stateId].distanceLevel;
    
    for (int i = 0; i < distancelevel[currentDlevel].iloscs; i++) {
        for (int j = 0; j < distancelevel[currentDlevel].subobiekty[i].iloscc; j++) {
            int prim_state = distancelevel[currentDlevel].subobiekty[i].czesci[j].prim_state_idx;

            if(primstate[prim_state].arg4 == -1){
                continue;
            } 
            ShapeTextureInfo *tInfo = new ShapeTextureInfo();
            tInfo->enabled = true;
            tInfo->textureName = image[texture[primstate[prim_state].arg4].image].name;
            tInfo->loaded = "NONE";
            list[image[texture[primstate[prim_state].arg4].image].tex] = tInfo;
            
            if (image[texture[primstate[prim_state].arg4].image].tex == -2){
                continue;
            }
            if (image[texture[primstate[prim_state].arg4].image].tex == -1){
                tInfo->loading = true;
                continue;
            }

            Texture* ttex = TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex];
            if(ttex == NULL)
                continue;     
            
            if (!ttex->loaded && !ttex->missing && !ttex->error ) {
                tInfo->loading = true;
                continue;
            }
            
            if(ttex->missing){
                tInfo->loaded = "MISSING";
                continue;
            }
            if(ttex->error){
                tInfo->loaded = "ERROR";
                continue;
            }
            
            if (!ttex->glLoaded) {
                tInfo->loading = true;
                continue;
            }
       
            tInfo->loaded = "YES";
            tInfo->resolution = QString::number(ttex->width) + "x" + QString::number(ttex->height);
            tInfo->textureId = image[texture[primstate[prim_state].arg4].image].tex;
        }
    }
}


/*======================================================
===== Przekształcenia takie tam
=======================================================*/
float* SFile::getPmatrix(int currentDlevel, float* pmatrix, int matrix) {
    if (matrix == -1 || matrix == 0) {
        Mat4::identity(pmatrix);
        pmatrix[0] = -1;
        return pmatrix;
    } else {
        pmatrix = getPmatrix(currentDlevel, pmatrix, distancelevel[currentDlevel].hierarchia[matrix]);
    }
    Mat4::multiply(pmatrix, pmatrix, macierz[matrix].param);
    return pmatrix;
}

float* SFile::getPmatrixAnimated(int currentDlevel, float* pmatrix, int matrix, float frame){
    if (matrix == -1 || matrix == 0) {
        Mat4::identity(pmatrix);
        pmatrix[0] = -1;
        return pmatrix;
    } else {
        pmatrix = getPmatrixAnimated(currentDlevel, pmatrix, distancelevel[currentDlevel].hierarchia[matrix], frame);
    }
    float m[16];
    memcpy(m, macierz[matrix].param, sizeof (float) * 16);
    if(animations[0].node.size() > matrix){
        AnimNode *n = &animations[0].node[matrix];
        if(frame + 1 < n->tcbId.size()){
            float m1[16];
            float q1[4];
            float q2[4];
            float q3[4];
            Quat::slerp(q1, n->tcbKey[n->tcbId[(int)frame].id1].quat, n->tcbKey[n->tcbId[(int)frame].id2].quat, n->tcbId[(int)frame].offset);
            Quat::slerp(q2, n->tcbKey[n->tcbId[(int)frame+1].id1].quat, n->tcbKey[n->tcbId[(int)frame+1].id2].quat, n->tcbId[(int)frame+1].offset);
            Quat::slerp(q3, q1, q2, frame - (int)frame);
            Mat4::fromQuat(m1, q3);
            Mat4::multiply(m, m, m1);
        }
        if(frame + 1 < n->linearId.size()){
            float v1[3], v2[3], v3[3];
            Vec3::lerp(v1, n->linearKey[n->linearId[(int)frame].id1].pos, n->linearKey[n->linearId[(int)frame].id2].pos, n->linearId[(int)frame].offset);
            Vec3::lerp(v2, n->linearKey[n->linearId[(int)frame+1].id1].pos, n->linearKey[n->linearId[(int)frame+1].id2].pos, n->linearId[(int)frame+1].offset);
            Vec3::lerp(v3, v1, v2, frame - (int)frame);
            
            m[12] = v3[0];
            m[13] = v3[1];
            m[14] = v3[2];
        }
        //slerprot
    }
    Mat4::multiply(pmatrix, pmatrix, m);
    return pmatrix;
}

void SFile::buildFrameIds(){
    for(int i = 0; i < animations.size(); i++){
        for(int j = 0; j < animations[i].node.size(); j++){
            int frameCount = animations[i].frames;
            AnimNode *n = &animations[i].node[j];

            for(int y = 0; y < frameCount + 1; y++){
                if(n->tcbKey.size() > 0){
                    n->tcbId.push_back(AnimFrameId());
                    n->tcbId.back().id1 = 0;
                    n->tcbId.back().id2 = 0;
                    n->tcbId.back().offset = 0;
                    //qDebug() << "n->tcbKey.size()"<< n->tcbKey.size();
                    for(int u = 0; u < n->tcbKey.size() - 1; u++){
                        if(n->tcbKey[u+1].frame > y){
                            n->tcbId.back().id1 = u;
                            n->tcbId.back().id2 = u+1;
                            n->tcbId.back().offset = ((float)y - (float)n->tcbKey[u].frame)/((float)n->tcbKey[u+1].frame - (float)n->tcbKey[u].frame);
                            break;
                        }
                    }
                }
                if(n->linearKey.size() > 0){
                    n->linearId.push_back(AnimFrameId());
                    n->linearId.back().id1 = 0;
                    n->linearId.back().id2 = 0;
                    n->linearId.back().offset = 0;
                    //qDebug() << "n->tcbKey.size()"<< n->tcbKey.size();
                    for(int u = 0; u < n->linearKey.size() - 1; u++){
                        if(n->linearKey[u+1].frame > y){
                            n->linearId.back().id1 = u;
                            n->linearId.back().id2 = u+1;
                            n->linearId.back().offset = ((float)y - (float)n->linearKey[u].frame)/((float)n->linearKey[u+1].frame - (float)n->linearKey[u].frame);
                            break;
                        }
                    }
                }
                //qDebug() << y << n->tcbId.back().id1 << n->tcbId.back().id2 << n->tcbId.back().offset;
/*
                n->linearId.push_back(AnimFrameId());
                n->linearId.back().id = 0;
                n->linearId.back().offset = 0;
                for(int u = 0; u < n->linearKey.size() - 1; u++){
                    if(n->linearKey[u+1].frame > y){
                        n->linearId.back().id = u;
                        n->linearId.back().offset = (n->linearKey[u+1].frame - n->linearKey[u].frame)/(y - n->linearKey[u].frame);
                        break;
                    }
                }

                n->slerpbId.push_back(AnimFrameId());
                n->slerpbId.back().id = 0;
                n->slerpbId.back().offset = 0;
                for(int u = 0; u < n->slerpRot.size() - 1; u++){
                    if(n->slerpRot[u+1].frame > y){
                        n->slerpbId.back().id = u;
                        n->slerpbId.back().offset = (n->slerpRot[u+1].frame - n->slerpRot[u].frame)/(y - n->slerpRot[u].frame);
                        break;
                    }
                }*/
            }
        }
    }
}