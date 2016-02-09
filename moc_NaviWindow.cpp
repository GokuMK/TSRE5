/****************************************************************************
** Meta object code from reading C++ file 'NaviWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NaviWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NaviWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NaviWindow_t {
    QByteArrayData data[13];
    char stringdata[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NaviWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NaviWindow_t qt_meta_stringdata_NaviWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "NaviWindow"
QT_MOC_LITERAL(1, 11, 6), // "jumpTo"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 22), // "PreciseTileCoordinate*"
QT_MOC_LITERAL(4, 42, 16), // "jumpTileSelected"
QT_MOC_LITERAL(5, 59, 8), // "naviInfo"
QT_MOC_LITERAL(6, 68, 3), // "all"
QT_MOC_LITERAL(7, 72, 6), // "hidden"
QT_MOC_LITERAL(8, 79, 7), // "posInfo"
QT_MOC_LITERAL(9, 87, 6), // "coords"
QT_MOC_LITERAL(10, 94, 13), // "latLonChanged"
QT_MOC_LITERAL(11, 108, 3), // "val"
QT_MOC_LITERAL(12, 112, 9) // "xyChanged"

    },
    "NaviWindow\0jumpTo\0\0PreciseTileCoordinate*\0"
    "jumpTileSelected\0naviInfo\0all\0hidden\0"
    "posInfo\0coords\0latLonChanged\0val\0"
    "xyChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NaviWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   47,    2, 0x0a /* Public */,
       5,    2,   48,    2, 0x0a /* Public */,
       8,    1,   53,    2, 0x0a /* Public */,
      10,    1,   56,    2, 0x0a /* Public */,
      12,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    6,    7,
    QMetaType::Void, 0x80000000 | 3,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,   11,

       0        // eod
};

void NaviWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NaviWindow *_t = static_cast<NaviWindow *>(_o);
        switch (_id) {
        case 0: _t->jumpTo((*reinterpret_cast< PreciseTileCoordinate*(*)>(_a[1]))); break;
        case 1: _t->jumpTileSelected(); break;
        case 2: _t->naviInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->posInfo((*reinterpret_cast< PreciseTileCoordinate*(*)>(_a[1]))); break;
        case 4: _t->latLonChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->xyChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NaviWindow::*_t)(PreciseTileCoordinate * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaviWindow::jumpTo)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject NaviWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_NaviWindow.data,
      qt_meta_data_NaviWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NaviWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NaviWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NaviWindow.stringdata))
        return static_cast<void*>(const_cast< NaviWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int NaviWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void NaviWindow::jumpTo(PreciseTileCoordinate * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
