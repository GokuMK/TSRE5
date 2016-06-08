/****************************************************************************
** Meta object code from reading C++ file 'ConListWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ConListWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConListWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConListWidget_t {
    QByteArrayData data[13];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConListWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConListWidget_t qt_meta_stringdata_ConListWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ConListWidget"
QT_MOC_LITERAL(1, 14, 15), // "conListSelected"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 2), // "id"
QT_MOC_LITERAL(4, 34, 3), // "aid"
QT_MOC_LITERAL(5, 38, 13), // "itemsSelected"
QT_MOC_LITERAL(6, 52, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(7, 69, 2), // "it"
QT_MOC_LITERAL(8, 72, 8), // "conFChan"
QT_MOC_LITERAL(9, 81, 1), // "n"
QT_MOC_LITERAL(10, 83, 8), // "conTChan"
QT_MOC_LITERAL(11, 92, 10), // "routeTChan"
QT_MOC_LITERAL(12, 103, 8) // "actTChan"

    },
    "ConListWidget\0conListSelected\0\0id\0aid\0"
    "itemsSelected\0QListWidgetItem*\0it\0"
    "conFChan\0n\0conTChan\0routeTChan\0actTChan"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConListWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       1,    2,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   57,    2, 0x0a /* Public */,
       8,    1,   60,    2, 0x0a /* Public */,
      10,    1,   63,    2, 0x0a /* Public */,
      11,    1,   66,    2, 0x0a /* Public */,
      12,    1,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QString,    9,

       0        // eod
};

void ConListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ConListWidget *_t = static_cast<ConListWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->conListSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->conListSelected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->itemsSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->conFChan((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->conTChan((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->routeTChan((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->actTChan((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConListWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConListWidget::conListSelected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ConListWidget::*_t)(int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConListWidget::conListSelected)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ConListWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConListWidget.data,
      qt_meta_data_ConListWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConListWidget.stringdata0))
        return static_cast<void*>(const_cast< ConListWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ConListWidget::conListSelected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConListWidget::conListSelected(int _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
