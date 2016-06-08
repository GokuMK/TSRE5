/****************************************************************************
** Meta object code from reading C++ file 'EngListWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "EngListWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EngListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_EngListWidget_t {
    QByteArrayData data[19];
    char stringdata0[197];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EngListWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EngListWidget_t qt_meta_stringdata_EngListWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "EngListWidget"
QT_MOC_LITERAL(1, 14, 15), // "engListSelected"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 2), // "id"
QT_MOC_LITERAL(4, 34, 16), // "addToConSelected"
QT_MOC_LITERAL(5, 51, 3), // "pos"
QT_MOC_LITERAL(6, 55, 5), // "count"
QT_MOC_LITERAL(7, 61, 2), // "f0"
QT_MOC_LITERAL(8, 64, 13), // "itemsSelected"
QT_MOC_LITERAL(9, 78, 20), // "addBegButtonSelected"
QT_MOC_LITERAL(10, 99, 20), // "addCurButtonSelected"
QT_MOC_LITERAL(11, 120, 20), // "addEndButtonSelected"
QT_MOC_LITERAL(12, 141, 20), // "addRndButtonSelected"
QT_MOC_LITERAL(13, 162, 2), // "fs"
QT_MOC_LITERAL(14, 165, 1), // "n"
QT_MOC_LITERAL(15, 167, 11), // "eventFilter"
QT_MOC_LITERAL(16, 179, 3), // "obj"
QT_MOC_LITERAL(17, 183, 7), // "QEvent*"
QT_MOC_LITERAL(18, 191, 5) // "event"

    },
    "EngListWidget\0engListSelected\0\0id\0"
    "addToConSelected\0pos\0count\0f0\0"
    "itemsSelected\0addBegButtonSelected\0"
    "addCurButtonSelected\0addEndButtonSelected\0"
    "addRndButtonSelected\0fs\0n\0eventFilter\0"
    "obj\0QEvent*\0event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EngListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    3,   67,    2, 0x06 /* Public */,
       7,    1,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   77,    2, 0x0a /* Public */,
       9,    0,   78,    2, 0x0a /* Public */,
      10,    0,   79,    2, 0x0a /* Public */,
      11,    0,   80,    2, 0x0a /* Public */,
      12,    0,   81,    2, 0x0a /* Public */,
      13,    1,   82,    2, 0x0a /* Public */,
      15,    2,   85,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    3,    5,    6,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Bool, QMetaType::QObjectStar, 0x80000000 | 17,   16,   18,

       0        // eod
};

void EngListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        EngListWidget *_t = static_cast<EngListWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->engListSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->addToConSelected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: _t->f0((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->itemsSelected(); break;
        case 4: _t->addBegButtonSelected(); break;
        case 5: _t->addCurButtonSelected(); break;
        case 6: _t->addEndButtonSelected(); break;
        case 7: _t->addRndButtonSelected(); break;
        case 8: _t->fs((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: { bool _r = _t->eventFilter((*reinterpret_cast< QObject*(*)>(_a[1])),(*reinterpret_cast< QEvent*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (EngListWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EngListWidget::engListSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (EngListWidget::*_t)(int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EngListWidget::addToConSelected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (EngListWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&EngListWidget::f0)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject EngListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_EngListWidget.data,
      qt_meta_data_EngListWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *EngListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EngListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_EngListWidget.stringdata0))
        return static_cast<void*>(const_cast< EngListWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int EngListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void EngListWidget::engListSelected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EngListWidget::addToConSelected(int _t1, int _t2, int _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EngListWidget::f0(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
