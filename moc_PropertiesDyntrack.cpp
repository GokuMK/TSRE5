/****************************************************************************
** Meta object code from reading C++ file 'PropertiesDyntrack.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PropertiesDyntrack.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PropertiesDyntrack.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PropertiesDyntrack_t {
    QByteArrayData data[14];
    char stringdata[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PropertiesDyntrack_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PropertiesDyntrack_t qt_meta_stringdata_PropertiesDyntrack = {
    {
QT_MOC_LITERAL(0, 0, 18), // "PropertiesDyntrack"
QT_MOC_LITERAL(1, 19, 10), // "enableTool"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "val"
QT_MOC_LITERAL(4, 35, 7), // "chSect1"
QT_MOC_LITERAL(5, 43, 7), // "chSect2"
QT_MOC_LITERAL(6, 51, 7), // "chSect3"
QT_MOC_LITERAL(7, 59, 7), // "chSect4"
QT_MOC_LITERAL(8, 67, 11), // "flexEnabled"
QT_MOC_LITERAL(9, 79, 8), // "flexData"
QT_MOC_LITERAL(10, 88, 1), // "x"
QT_MOC_LITERAL(11, 90, 1), // "z"
QT_MOC_LITERAL(12, 92, 6), // "float*"
QT_MOC_LITERAL(13, 99, 1) // "p"

    },
    "PropertiesDyntrack\0enableTool\0\0val\0"
    "chSect1\0chSect2\0chSect3\0chSect4\0"
    "flexEnabled\0flexData\0x\0z\0float*\0p"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PropertiesDyntrack[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   52,    2, 0x0a /* Public */,
       5,    0,   53,    2, 0x0a /* Public */,
       6,    0,   54,    2, 0x0a /* Public */,
       7,    0,   55,    2, 0x0a /* Public */,
       8,    0,   56,    2, 0x0a /* Public */,
       9,    3,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, 0x80000000 | 12,   10,   11,   13,

       0        // eod
};

void PropertiesDyntrack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PropertiesDyntrack *_t = static_cast<PropertiesDyntrack *>(_o);
        switch (_id) {
        case 0: _t->enableTool((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->chSect1(); break;
        case 2: _t->chSect2(); break;
        case 3: _t->chSect3(); break;
        case 4: _t->chSect4(); break;
        case 5: _t->flexEnabled(); break;
        case 6: _t->flexData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (PropertiesDyntrack::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&PropertiesDyntrack::enableTool)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject PropertiesDyntrack::staticMetaObject = {
    { &PropertiesAbstract::staticMetaObject, qt_meta_stringdata_PropertiesDyntrack.data,
      qt_meta_data_PropertiesDyntrack,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PropertiesDyntrack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PropertiesDyntrack::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PropertiesDyntrack.stringdata))
        return static_cast<void*>(const_cast< PropertiesDyntrack*>(this));
    return PropertiesAbstract::qt_metacast(_clname);
}

int PropertiesDyntrack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = PropertiesAbstract::qt_metacall(_c, _id, _a);
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
void PropertiesDyntrack::enableTool(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
