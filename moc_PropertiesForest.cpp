/****************************************************************************
** Meta object code from reading C++ file 'PropertiesForest.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PropertiesForest.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropertiesForest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PropertiesForest_t {
    QByteArrayData data[6];
    char stringdata0[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PropertiesForest_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PropertiesForest_t qt_meta_stringdata_PropertiesForest = {
    {
QT_MOC_LITERAL(0, 0, 16), // "PropertiesForest"
QT_MOC_LITERAL(1, 17, 11), // "sizeEnabled"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 3), // "val"
QT_MOC_LITERAL(4, 34, 17), // "populationEnabled"
QT_MOC_LITERAL(5, 52, 16) // "densitykmEnabled"

    },
    "PropertiesForest\0sizeEnabled\0\0val\0"
    "populationEnabled\0densitykmEnabled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropertiesForest[] = {

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
       1,    1,   29,    2, 0x0a /* Public */,
       4,    1,   32,    2, 0x0a /* Public */,
       5,    1,   35,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void PropertiesForest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PropertiesForest *_t = static_cast<PropertiesForest *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sizeEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->populationEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->densitykmEnabled((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PropertiesForest::staticMetaObject = {
    { &PropertiesAbstract::staticMetaObject, qt_meta_stringdata_PropertiesForest.data,
      qt_meta_data_PropertiesForest,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PropertiesForest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertiesForest::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PropertiesForest.stringdata0))
        return static_cast<void*>(const_cast< PropertiesForest*>(this));
    return PropertiesAbstract::qt_metacast(_clname);
}

int PropertiesForest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
