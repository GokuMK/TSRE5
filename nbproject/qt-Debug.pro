# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/MinGW_QT-Windows
TARGET = TSRE5
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += debug 
PKGCONFIG +=
QT = core gui widgets opengl
SOURCES += AceLib.cpp Camera.cpp DynTrackObj.cpp Eng.cpp FileBuffer.cpp FileFunctions.cpp ForestObj.cpp GLH.cpp GLMatrix.cpp GLUU.cpp Game.cpp LevelCrObj.cpp LoadWindow.cpp NaviBox.cpp OglObj.cpp PaintTexLib.cpp ParserX.cpp Path.cpp PickupObj.cpp PlatformObj.cpp Pointer3d.cpp ReadFile.cpp Ref.cpp Route.cpp Ruch.cpp SFile.cpp SFileC.cpp SFileX.cpp ShapeLib.cpp SignalObj.cpp SpeedpostObj.cpp StaticObj.cpp TDB.cpp TFile.cpp TRnode.cpp TSection.cpp TSectionDAT.cpp Terrain.cpp TerrainLib.cpp TexLib.cpp TextObj.cpp Texture.cpp Tile.cpp ToolBox.cpp TrWatermarkObj.cpp TrackObj.cpp TrackShape.cpp TransferObj.cpp Vector2f.cpp Vector2i.cpp Vector3f.cpp Vector4f.cpp WorldObj.cpp glwidget.cpp main.cpp window.cpp
HEADERS += AceLib.h Camera.h DynTrackObj.h Eng.h FileBuffer.h FileFunctions.h ForestObj.h GLH.h GLMatrix.h GLUU.h Game.h LevelCrObj.h LoadWindow.h NaviBox.h OglObj.h PaintTexLib.h ParserX.h Path.h PickupObj.h PlatformObj.h Pointer3d.h ReadFile.h Ref.h Route.h Ruch.h SFile.h SFileC.h SFileX.h ShapeLib.h SignalObj.h SpeedpostObj.h StaticObj.h TDB.h TFile.h TRnode.h TSection.h TSectionDAT.h Terrain.h TerrainLib.h TexLib.h TextObj.h Texture.h Tile.h ToolBox.h TrWatermarkObj.h TrackObj.h TrackShape.h TransferObj.h Vector2f.h Vector2i.h Vector3f.h Vector4f.h WorldObj.h glwidget.h window.h zconf.h zlib.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/MinGW_QT-Windows
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += -L C:\Users\Goku\Desktop\Programy\zlib128-dll -lzlib1  
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
