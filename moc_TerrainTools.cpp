/****************************************************************************
** Meta object code from reading C++ file 'TerrainTools.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "TerrainTools.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TerrainTools.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TerrainTools_t {
    QByteArrayData data[18];
    char stringdata[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TerrainTools_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TerrainTools_t qt_meta_stringdata_TerrainTools = {
    {
QT_MOC_LITERAL(0, 0, 12), // "TerrainTools"
QT_MOC_LITERAL(1, 13, 10), // "enableTool"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 4), // "name"
QT_MOC_LITERAL(4, 30, 13), // "setPaintBrush"
QT_MOC_LITERAL(5, 44, 6), // "Brush*"
QT_MOC_LITERAL(6, 51, 5), // "brush"
QT_MOC_LITERAL(7, 57, 17), // "heightToolEnabled"
QT_MOC_LITERAL(8, 75, 16), // "paintToolEnabled"
QT_MOC_LITERAL(9, 92, 18), // "pickTexToolEnabled"
QT_MOC_LITERAL(10, 111, 17), // "setTexToolEnabled"
QT_MOC_LITERAL(11, 129, 17), // "putTexToolEnabled"
QT_MOC_LITERAL(12, 147, 18), // "chooseColorEnabled"
QT_MOC_LITERAL(13, 166, 13), // "updateTexPrev"
QT_MOC_LITERAL(14, 180, 12), // "setBrushSize"
QT_MOC_LITERAL(15, 193, 3), // "val"
QT_MOC_LITERAL(16, 197, 13), // "setBrushAlpha"
QT_MOC_LITERAL(17, 211, 17) // "setBrushTextureId"

    },
    "TerrainTools\0enableTool\0\0name\0"
    "setPaintBrush\0Brush*\0brush\0heightToolEnabled\0"
    "paintToolEnabled\0pickTexToolEnabled\0"
    "setTexToolEnabled\0putTexToolEnabled\0"
    "chooseColorEnabled\0updateTexPrev\0"
    "setBrushSize\0val\0setBrushAlpha\0"
    "setBrushTextureId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TerrainTools[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       4,    1,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   80,    2, 0x0a /* Public */,
       8,    0,   81,    2, 0x0a /* Public */,
       9,    0,   82,    2, 0x0a /* Public */,
      10,    0,   83,    2, 0x0a /* Public */,
      11,    0,   84,    2, 0x0a /* Public */,
      12,    0,   85,    2, 0x0a /* Public */,
      13,    0,   86,    2, 0x0a /* Public */,
      14,    1,   87,    2, 0x0a /* Public */,
      16,    1,   90,    2, 0x0a /* Public */,
      17,    1,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

void TerrainTools::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TerrainTools *_t = static_cast<TerrainTools *>(_o);
        switch (_id) {
        case 0: _t->enableTool((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->setPaintBrush((*reinterpret_cast< Brush*(*)>(_a[1]))); break;
        case 2: _t->heightToolEnabled(); break;
        case 3: _t->paintToolEnabled(); break;
        case 4: _t->pickTexToolEnabled(); break;
        case 5: _t->setTexToolEnabled(); break;
        case 6: _t->putTexToolEnabled(); break;
        case 7: _t->chooseColorEnabled(); break;
        case 8: _t->updateTexPrev(); break;
        case 9: _t->setBrushSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->setBrushAlpha((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setBrushTextureId((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TerrainTools::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TerrainTools::enableTool)) {
                *result = 0;
            }
        }
        {
            typedef void (TerrainTools::*_t)(Brush * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TerrainTools::setPaintBrush)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject TerrainTools::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TerrainTools.data,
      qt_meta_data_TerrainTools,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TerrainTools::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TerrainTools::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TerrainTools.stringdata))
        return static_cast<void*>(const_cast< TerrainTools*>(this));
    return QWidget::qt_metacast(_clname);
}

int TerrainTools::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void TerrainTools::enableTool(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TerrainTools::setPaintBrush(Brush * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
