/****************************************************************************
** Meta object code from reading C++ file 'NaviBox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NaviBox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NaviBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NaviBox_t {
    QByteArrayData data[10];
    char stringdata0[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NaviBox_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NaviBox_t qt_meta_stringdata_NaviBox = {
    {
QT_MOC_LITERAL(0, 0, 7), // "NaviBox"
QT_MOC_LITERAL(1, 8, 6), // "jumpTo"
QT_MOC_LITERAL(2, 15, 0), // ""
QT_MOC_LITERAL(3, 16, 1), // "x"
QT_MOC_LITERAL(4, 18, 1), // "y"
QT_MOC_LITERAL(5, 20, 16), // "jumpTileSelected"
QT_MOC_LITERAL(6, 37, 8), // "naviInfo"
QT_MOC_LITERAL(7, 46, 1), // "z"
QT_MOC_LITERAL(8, 48, 3), // "all"
QT_MOC_LITERAL(9, 52, 6) // "hidden"

    },
    "NaviBox\0jumpTo\0\0x\0y\0jumpTileSelected\0"
    "naviInfo\0z\0all\0hidden"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NaviBox[] = {

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
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   34,    2, 0x0a /* Public */,
       6,    4,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    7,    8,    9,

       0        // eod
};

void NaviBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NaviBox *_t = static_cast<NaviBox *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->jumpTo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->jumpTileSelected(); break;
        case 2: _t->naviInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NaviBox::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaviBox::jumpTo)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject NaviBox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_NaviBox.data,
      qt_meta_data_NaviBox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NaviBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NaviBox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NaviBox.stringdata0))
        return static_cast<void*>(const_cast< NaviBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int NaviBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void NaviBox::jumpTo(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
