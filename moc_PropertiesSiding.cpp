/****************************************************************************
** Meta object code from reading C++ file 'PropertiesSiding.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PropertiesSiding.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropertiesSiding.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PropertiesSiding_t {
    QByteArrayData data[6];
    char stringdata0[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PropertiesSiding_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PropertiesSiding_t qt_meta_stringdata_PropertiesSiding = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PropertiesSiding"
QT_MOC_LITERAL(1, 17, 22), // "disablePlatformEnabled"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 5), // "state"
QT_MOC_LITERAL(4, 47, 19), // "namePlatformEnabled"
QT_MOC_LITERAL(5, 67, 3) // "val"

    },
    "PropertiesSiding\0disablePlatformEnabled\0"
    "\0state\0namePlatformEnabled\0val"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropertiesSiding[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       4,    1,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void PropertiesSiding::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PropertiesSiding *_t = static_cast<PropertiesSiding *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->disablePlatformEnabled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->namePlatformEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PropertiesSiding::staticMetaObject = {
    { &PropertiesAbstract::staticMetaObject, qt_meta_stringdata_PropertiesSiding.data,
      qt_meta_data_PropertiesSiding,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PropertiesSiding::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertiesSiding::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PropertiesSiding.stringdata0))
        return static_cast<void*>(const_cast< PropertiesSiding*>(this));
    return PropertiesAbstract::qt_metacast(_clname);
}

int PropertiesSiding::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PropertiesAbstract::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
