/****************************************************************************
** Meta object code from reading C++ file 'PropertiesPlatform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PropertiesPlatform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropertiesPlatform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PropertiesPlatform_t {
    QByteArrayData data[12];
    char stringdata0[170];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PropertiesPlatform_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PropertiesPlatform_t qt_meta_stringdata_PropertiesPlatform = {
    {
QT_MOC_LITERAL(0, 0, 18), // "PropertiesPlatform"
QT_MOC_LITERAL(1, 19, 15), // "leftSideEnabled"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 5), // "state"
QT_MOC_LITERAL(4, 42, 16), // "rightSideEnabled"
QT_MOC_LITERAL(5, 59, 22), // "disablePlatformEnabled"
QT_MOC_LITERAL(6, 82, 18), // "nameStationEnabled"
QT_MOC_LITERAL(7, 101, 3), // "val"
QT_MOC_LITERAL(8, 105, 19), // "namePlatformEnabled"
QT_MOC_LITERAL(9, 125, 14), // "waitMinEnabled"
QT_MOC_LITERAL(10, 140, 14), // "waitSecEnabled"
QT_MOC_LITERAL(11, 155, 14) // "waitPasEnabled"

    },
    "PropertiesPlatform\0leftSideEnabled\0\0"
    "state\0rightSideEnabled\0disablePlatformEnabled\0"
    "nameStationEnabled\0val\0namePlatformEnabled\0"
    "waitMinEnabled\0waitSecEnabled\0"
    "waitPasEnabled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropertiesPlatform[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x0a /* Public */,
       4,    1,   57,    2, 0x0a /* Public */,
       5,    1,   60,    2, 0x0a /* Public */,
       6,    1,   63,    2, 0x0a /* Public */,
       8,    1,   66,    2, 0x0a /* Public */,
       9,    1,   69,    2, 0x0a /* Public */,
      10,    1,   72,    2, 0x0a /* Public */,
      11,    1,   75,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,

       0        // eod
};

void PropertiesPlatform::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PropertiesPlatform *_t = static_cast<PropertiesPlatform *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->leftSideEnabled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->rightSideEnabled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->disablePlatformEnabled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->nameStationEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->namePlatformEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->waitMinEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->waitSecEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->waitPasEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PropertiesPlatform::staticMetaObject = {
    { &PropertiesAbstract::staticMetaObject, qt_meta_stringdata_PropertiesPlatform.data,
      qt_meta_data_PropertiesPlatform,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PropertiesPlatform::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertiesPlatform::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PropertiesPlatform.stringdata0))
        return static_cast<void*>(const_cast< PropertiesPlatform*>(this));
    return PropertiesAbstract::qt_metacast(_clname);
}

int PropertiesPlatform::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PropertiesAbstract::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
