# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/MinGW_QT_static-Windows
TARGET = TSRE5
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
PKGCONFIG +=
QT = core gui widgets opengl
SOURCES += AboutWindow.cpp AceLib.cpp Brush.cpp Camera.cpp DynTrackObj.cpp Eng.cpp FileBuffer.cpp FileFunctions.cpp Flex.cpp ForestObj.cpp GLH.cpp GLMatrix.cpp GLUU.cpp Game.cpp GuiFunct.cpp HazardObj.cpp ImageLib.cpp Intersections.cpp LevelCrObj.cpp LoadWindow.cpp NaviBox.cpp ObjTools.cpp OglObj.cpp PaintTexLib.cpp ParserX.cpp Path.cpp PickupObj.cpp PlatformObj.cpp Pointer3d.cpp PropertiesAbstract.cpp PropertiesCarspawner.cpp PropertiesDyntrack.cpp PropertiesPickup.cpp PropertiesPlatform.cpp PropertiesSiding.cpp PropertiesSignal.cpp PropertiesStatic.cpp PropertiesTransfer.cpp PropertiesUndefined.cpp ReadFile.cpp Ref.cpp Route.cpp Ruch.cpp SFile.cpp SFileC.cpp SFileX.cpp ShapeLib.cpp SigCfg.cpp SignalObj.cpp SignalShape.cpp SignalType.cpp SpeedpostObj.cpp StaticObj.cpp TDB.cpp TFile.cpp TRitem.cpp TRnode.cpp TS.cpp TSection.cpp TSectionDAT.cpp Terrain.cpp TerrainLib.cpp TerrainTools.cpp TexLib.cpp TextObj.cpp Texture.cpp Tile.cpp TrWatermarkObj.cpp TrackItemObj.cpp TrackObj.cpp TrackShape.cpp TransferObj.cpp Vector2f.cpp Vector2i.cpp Vector3f.cpp Vector4f.cpp WorldObj.cpp glwidget.cpp main.cpp window.cpp
HEADERS += AboutWindow.h AceLib.h Brush.h Camera.h DynTrackObj.h Eng.h FileBuffer.h FileFunctions.h Flex.h ForestObj.h GLH.h GLMatrix.h GLUU.h Game.h GuiFunct.h HazardObj.h ImageLib.h Intersections.h LevelCrObj.h LoadWindow.h NaviBox.h ObjTools.h OglObj.h PaintTexLib.h ParserX.h Path.h PickupObj.h PlatformObj.h Pointer3d.h PropertiesAbstract.h PropertiesCarspawner.h PropertiesDyntrack.h PropertiesPickup.h PropertiesPlatform.h PropertiesSiding.h PropertiesSignal.h PropertiesStatic.h PropertiesTransfer.h PropertiesUndefined.h ReadFile.h Ref.h Route.h Ruch.h SFile.h SFileC.h SFileX.h ShapeLib.h SigCfg.h SignalObj.h SignalShape.h SignalType.h SpeedpostObj.h StaticObj.h TDB.h TFile.h TRitem.h TRnode.h TS.h TSection.h TSectionDAT.h Terrain.h TerrainLib.h TerrainTools.h TexLib.h TextObj.h Texture.h Tile.h TrWatermarkObj.h TrackItemObj.h TrackObj.h TrackShape.h TransferObj.h Vector2f.h Vector2i.h Vector3f.h Vector4f.h WorldObj.h glwidget.h window.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Release/MinGW_QT_static-Windows
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
CONFIG += console
