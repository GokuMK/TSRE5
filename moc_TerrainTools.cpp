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
    QByteArrayData data[6];
    char stringdata[65];
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
QT_MOC_LITERAL(4, 30, 17), // "heightToolEnabled"
QT_MOC_LITERAL(5, 48, 16) // "paintToolEnabled"

    },
    "TerrainTools\0enableTool\0\0name\0"
    "heightToolEnabled\0paintToolEnabled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TerrainTools[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   32,    2, 0x0a /* Public */,
       5,    0,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TerrainTools::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TerrainTools *_t = static_cast<TerrainTools *>(_o);
        switch (_id) {
        case 0: _t->enableTool((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->heightToolEnabled(); break;
        case 2: _t->paintToolEnabled(); break;
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
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TerrainTools::enableTool(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
