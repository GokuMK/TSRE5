# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/dynamic_release/MinGW_QT_5.7.0-Windows
TARGET = TSRE5
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
PKGCONFIG +=
QT = core gui widgets network opengl
SOURCES += AboutWindow.cpp AceLib.cpp ActLib.cpp Activity.cpp ActivityTools.cpp Brush.cpp CELoadWindow.cpp Camera.cpp CameraConsist.cpp CameraFree.cpp CameraRot.cpp ChooseFileDialog.cpp ConEditorWindow.cpp ConLib.cpp ConListWidget.cpp ConUnitsWidget.cpp Consist.cpp Coords.cpp CoordsGpx.cpp CoordsKml.cpp CoordsMkr.cpp DynTrackObj.cpp EditFileNameDialog.cpp Eng.cpp EngLib.cpp EngListWidget.cpp Features.cpp FileBuffer.cpp FileFunctions.cpp Flex.cpp ForestObj.cpp GLH.cpp GLMatrix.cpp GLUU.cpp Game.cpp GeoTools.cpp GroupObj.cpp GuiFunct.cpp HGTfile.cpp HazardObj.cpp HeightWindow.cpp IghCoords.cpp ImageLib.cpp Intersections.cpp LevelCrObj.cpp LoadWindow.cpp MapLib.cpp MapWindow.cpp NaviBox.cpp NaviWindow.cpp NewRouteWindow.cpp OSM.cpp ObjTools.cpp OglObj.cpp OverwriteDialog.cpp PaintTexLib.cpp ParserX.cpp Path.cpp PickupObj.cpp PlatformObj.cpp Pointer3d.cpp PoleObj.cpp PropertiesAbstract.cpp PropertiesCarspawner.cpp PropertiesDyntrack.cpp PropertiesForest.cpp PropertiesGroup.cpp PropertiesPickup.cpp PropertiesPlatform.cpp PropertiesSiding.cpp PropertiesSignal.cpp PropertiesSoundSource.cpp PropertiesSpeedpost.cpp PropertiesStatic.cpp PropertiesTrackObj.cpp PropertiesTransfer.cpp PropertiesUndefined.cpp QuadTree.cpp RandomConsist.cpp ReadFile.cpp Ref.cpp Route.cpp Ruch.cpp SFile.cpp SFileC.cpp SFileX.cpp Shader.cpp ShapeLib.cpp ShapeViewWindow.cpp SigCfg.cpp SignalObj.cpp SignalShape.cpp SignalType.cpp SignalWindow.cpp SignalWindowLink.cpp SoundList.cpp SoundRegionObj.cpp SoundSourceObj.cpp SpeedPost.cpp SpeedPostDAT.cpp SpeedpostObj.cpp StaticObj.cpp TDB.cpp TFile.cpp TRitem.cpp TRnode.cpp TS.cpp TSection.cpp TSectionDAT.cpp Terrain.cpp TerrainLib.cpp TerrainTools.cpp TerrainTreeWindow.cpp TerrainWaterWindow.cpp TexLib.cpp TextObj.cpp Texture.cpp Tile.cpp TrWatermarkObj.cpp TrackItemObj.cpp TrackObj.cpp TrackShape.cpp TransferObj.cpp TransformWorldObjDialog.cpp Trk.cpp TrkWindow.cpp UnsavedDialog.cpp Vector2f.cpp Vector2i.cpp Vector3f.cpp Vector4f.cpp WorldObj.cpp glShapeWidget.cpp glwidget.cpp main.cpp window.cpp
HEADERS += AboutWindow.h AceLib.h ActLib.h Activity.h ActivityTools.h Brush.h CELoadWindow.h Camera.h CameraConsist.h CameraFree.h CameraRot.h ChooseFileDialog.h ConEditorWindow.h ConLib.h ConListWidget.h ConUnitsWidget.h Consist.h Coords.h CoordsGpx.h CoordsKml.h CoordsMkr.h DynTrackObj.h EditFileNameDialog.h Eng.h EngLib.h EngListWidget.h Features.h FileBuffer.h FileFunctions.h Flex.h ForestObj.h GLH.h GLMatrix.h GLUU.h Game.h GeoTools.h GroupObj.h GuiFunct.h HGTfile.h HazardObj.h HeightWindow.h IghCoords.h ImageLib.h Intersections.h LevelCrObj.h LoadWindow.h MapLib.h MapWindow.h NaviBox.h NaviWindow.h NewRouteWindow.h OSM.h ObjTools.h OglObj.h OverwriteDialog.h PaintTexLib.h ParserX.h Path.h PickupObj.h PlatformObj.h Pointer3d.h PoleObj.h PropertiesAbstract.h PropertiesCarspawner.h PropertiesDyntrack.h PropertiesForest.h PropertiesGroup.h PropertiesPickup.h PropertiesPlatform.h PropertiesSiding.h PropertiesSignal.h PropertiesSoundSource.h PropertiesSpeedpost.h PropertiesStatic.h PropertiesTrackObj.h PropertiesTransfer.h PropertiesUndefined.h QuadTree.h RandomConsist.h ReadFile.h Ref.h Route.h Ruch.h SFile.h SFileC.h SFileX.h Shader.h ShapeLib.h ShapeViewWindow.h SigCfg.h SignalObj.h SignalShape.h SignalType.h SignalWindow.h SignalWindowLink.h SoundList.h SoundRegionObj.h SoundSourceObj.h SpeedPost.h SpeedPostDAT.h SpeedpostObj.h StaticObj.h TDB.h TFile.h TRitem.h TRnode.h TS.h TSection.h TSectionDAT.h Terrain.h TerrainLib.h TerrainTools.h TerrainTreeWindow.h TerrainWaterWindow.h TexLib.h TextObj.h Texture.h Tile.h TrWatermarkObj.h TrackItemObj.h TrackObj.h TrackShape.h TransferObj.h TransformWorldObjDialog.h Trk.h TrkWindow.h UnsavedDialog.h Vector2f.h Vector2i.h Vector3f.h Vector4f.h WorldObj.h glShapeWidget.h glwidget.h window.h
FORMS +=
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/dynamic_release/MinGW_QT_5.7.0-Windows
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += -lopengl32   
equals(QT_MAJOR_VERSION, 4) {
QMAKE_CXXFLAGS += -std=c++11
}
equals(QT_MAJOR_VERSION, 5) {
CONFIG += c++11
}
"CONFIG += console"
