/****************************************************************************
** Meta object code from reading C++ file 'PropertiesSignal.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PropertiesSignal.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropertiesSignal.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PropertiesSignal_t {
    QByteArrayData data[5];
    char stringdata[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PropertiesSignal_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PropertiesSignal_t qt_meta_stringdata_PropertiesSignal = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PropertiesSignal"
QT_MOC_LITERAL(1, 17, 12), // "chSubEnabled"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "idx"
QT_MOC_LITERAL(4, 35, 10) // "flipSignal"

    },
    "PropertiesSignal\0chSubEnabled\0\0idx\0"
    "flipSignal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropertiesSignal[] = {

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
       4,    0,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

       0        // eod
};

void PropertiesSignal::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PropertiesSignal *_t = static_cast<PropertiesSignal *>(_o);
        switch (_id) {
        case 0: _t->chSubEnabled((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->flipSignal(); break;
        default: ;
        }
    }
}

const QMetaObject PropertiesSignal::staticMetaObject = {
    { &PropertiesAbstract::staticMetaObject, qt_meta_stringdata_PropertiesSignal.data,
      qt_meta_data_PropertiesSignal,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PropertiesSignal::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertiesSignal::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PropertiesSignal.stringdata))
        return static_cast<void*>(const_cast< PropertiesSignal*>(this));
    return PropertiesAbstract::qt_metacast(_clname);
}

int PropertiesSignal::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
