/****************************************************************************
** Meta object code from reading C++ file 'NaviWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "NaviWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NaviWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_NaviWindow_t {
    QByteArrayData data[21];
    char stringdata0[229];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NaviWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NaviWindow_t qt_meta_stringdata_NaviWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "NaviWindow"
QT_MOC_LITERAL(1, 11, 6), // "jumpTo"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 22), // "PreciseTileCoordinate*"
QT_MOC_LITERAL(4, 42, 7), // "sendMsg"
QT_MOC_LITERAL(5, 50, 4), // "name"
QT_MOC_LITERAL(6, 55, 3), // "val"
QT_MOC_LITERAL(7, 59, 16), // "jumpTileSelected"
QT_MOC_LITERAL(8, 76, 8), // "naviInfo"
QT_MOC_LITERAL(9, 85, 3), // "all"
QT_MOC_LITERAL(10, 89, 6), // "hidden"
QT_MOC_LITERAL(11, 96, 7), // "posInfo"
QT_MOC_LITERAL(12, 104, 6), // "coords"
QT_MOC_LITERAL(13, 111, 13), // "latLonChanged"
QT_MOC_LITERAL(14, 125, 9), // "xyChanged"
QT_MOC_LITERAL(15, 135, 7), // "mkrList"
QT_MOC_LITERAL(16, 143, 42), // "std::unordered_map<std::strin..."
QT_MOC_LITERAL(17, 186, 4), // "list"
QT_MOC_LITERAL(18, 191, 16), // "mkrFilesSelected"
QT_MOC_LITERAL(19, 208, 4), // "item"
QT_MOC_LITERAL(20, 213, 15) // "mkrListSelected"

    },
    "NaviWindow\0jumpTo\0\0PreciseTileCoordinate*\0"
    "sendMsg\0name\0val\0jumpTileSelected\0"
    "naviInfo\0all\0hidden\0posInfo\0coords\0"
    "latLonChanged\0xyChanged\0mkrList\0"
    "std::unordered_map<std::string,CoordsMkr*>\0"
    "list\0mkrFilesSelected\0item\0mkrListSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NaviWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       4,    2,   90,    2, 0x06 /* Public */,
       4,    2,   95,    2, 0x06 /* Public */,
       4,    2,  100,    2, 0x06 /* Public */,
       4,    2,  105,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,  110,    2, 0x0a /* Public */,
       8,    2,  111,    2, 0x0a /* Public */,
      11,    1,  116,    2, 0x0a /* Public */,
      13,    1,  119,    2, 0x0a /* Public */,
      14,    1,  122,    2, 0x0a /* Public */,
      15,    1,  125,    2, 0x0a /* Public */,
      18,    1,  128,    2, 0x0a /* Public */,
      20,    1,  131,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Float,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,   10,
    QMetaType::Void, 0x80000000 | 3,   12,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::QString,   19,

       0        // eod
};

void NaviWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NaviWindow *_t = static_cast<NaviWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->jumpTo((*reinterpret_cast< PreciseTileCoordinate*(*)>(_a[1]))); break;
        case 1: _t->sendMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sendMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->sendMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->sendMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 5: _t->sendMsg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->jumpTileSelected(); break;
        case 7: _t->naviInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->posInfo((*reinterpret_cast< PreciseTileCoordinate*(*)>(_a[1]))); break;
        case 9: _t->latLonChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->xyChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->mkrList((*reinterpret_cast< std::unordered_map<std::string,CoordsMkr*>(*)>(_a[1]))); break;
        case 12: _t->mkrFilesSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->mkrListSelected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (NaviWindow::*_t)(PreciseTileCoordinate * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaviWindow::jumpTo)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (NaviWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaviWindow::sendMsg)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (NaviWindow::*_t)(QString , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaviWindow::sendMsg)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (NaviWindow::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaviWindow::sendMsg)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (NaviWindow::*_t)(QString , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaviWindow::sendMsg)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (NaviWindow::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&NaviWindow::sendMsg)) {
                *result = 5;
                return;
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
    if (!strcmp(_clname, qt_meta_stringdata_NaviWindow.stringdata0))
        return static_cast<void*>(const_cast< NaviWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int NaviWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void NaviWindow::jumpTo(PreciseTileCoordinate * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NaviWindow::sendMsg(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NaviWindow::sendMsg(QString _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NaviWindow::sendMsg(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NaviWindow::sendMsg(QString _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NaviWindow::sendMsg(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
