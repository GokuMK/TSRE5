/****************************************************************************
** Meta object code from reading C++ file 'PropertiesSpeedpost.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PropertiesSpeedpost.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropertiesSpeedpost.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PropertiesSpeedpost_t {
    QByteArrayData data[6];
    char stringdata0[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PropertiesSpeedpost_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PropertiesSpeedpost_t qt_meta_stringdata_PropertiesSpeedpost = {
    {
QT_MOC_LITERAL(0, 0, 19), // "PropertiesSpeedpost"
QT_MOC_LITERAL(1, 20, 10), // "flipSignal"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "speedEnabled"
QT_MOC_LITERAL(4, 45, 3), // "val"
QT_MOC_LITERAL(5, 49, 13) // "numberEnabled"

    },
    "PropertiesSpeedpost\0flipSignal\0\0"
    "speedEnabled\0val\0numberEnabled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropertiesSpeedpost[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    1,   30,    2, 0x0a /* Public */,
       5,    1,   33,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void PropertiesSpeedpost::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PropertiesSpeedpost *_t = static_cast<PropertiesSpeedpost *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->flipSignal(); break;
        case 1: _t->speedEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->numberEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PropertiesSpeedpost::staticMetaObject = {
    { &PropertiesAbstract::staticMetaObject, qt_meta_stringdata_PropertiesSpeedpost.data,
      qt_meta_data_PropertiesSpeedpost,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PropertiesSpeedpost::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertiesSpeedpost::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PropertiesSpeedpost.stringdata0))
        return static_cast<void*>(const_cast< PropertiesSpeedpost*>(this));
    return PropertiesAbstract::qt_metacast(_clname);
}

int PropertiesSpeedpost::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PropertiesAbstract::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
