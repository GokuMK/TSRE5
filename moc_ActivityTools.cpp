/****************************************************************************
** Meta object code from reading C++ file 'ActivityTools.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ActivityTools.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ActivityTools.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ActivityTools_t {
    QByteArrayData data[8];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ActivityTools_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ActivityTools_t qt_meta_stringdata_ActivityTools = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ActivityTools"
QT_MOC_LITERAL(1, 14, 12), // "loadActFiles"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 14), // "fillConListAct"
QT_MOC_LITERAL(4, 43, 1), // "n"
QT_MOC_LITERAL(5, 45, 11), // "routeLoaded"
QT_MOC_LITERAL(6, 57, 6), // "Route*"
QT_MOC_LITERAL(7, 64, 1) // "r"

    },
    "ActivityTools\0loadActFiles\0\0fillConListAct\0"
    "n\0routeLoaded\0Route*\0r"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ActivityTools[] = {

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
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void ActivityTools::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ActivityTools *_t = static_cast<ActivityTools *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadActFiles(); break;
        case 1: _t->fillConListAct((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->routeLoaded((*reinterpret_cast< Route*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ActivityTools::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ActivityTools.data,
      qt_meta_data_ActivityTools,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ActivityTools::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ActivityTools::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ActivityTools.stringdata0))
        return static_cast<void*>(const_cast< ActivityTools*>(this));
    return QWidget::qt_metacast(_clname);
}

int ActivityTools::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
