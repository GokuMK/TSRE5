/****************************************************************************
** Meta object code from reading C++ file 'ObjTools.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ObjTools.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ObjTools.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ObjTools_t {
    QByteArrayData data[19];
    char stringdata0[226];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ObjTools_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ObjTools_t qt_meta_stringdata_ObjTools = {
    {
QT_MOC_LITERAL(0, 0, 8), // "ObjTools"
QT_MOC_LITERAL(1, 9, 10), // "enableTool"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 4), // "name"
QT_MOC_LITERAL(4, 26, 11), // "routeLoaded"
QT_MOC_LITERAL(5, 38, 6), // "Route*"
QT_MOC_LITERAL(6, 45, 1), // "a"
QT_MOC_LITERAL(7, 47, 16), // "refClassSelected"
QT_MOC_LITERAL(8, 64, 4), // "text"
QT_MOC_LITERAL(9, 69, 16), // "refTrackSelected"
QT_MOC_LITERAL(10, 86, 15), // "refListSelected"
QT_MOC_LITERAL(11, 102, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(12, 119, 4), // "item"
QT_MOC_LITERAL(13, 124, 17), // "trackListSelected"
QT_MOC_LITERAL(14, 142, 21), // "lastItemsListSelected"
QT_MOC_LITERAL(15, 164, 17), // "selectToolEnabled"
QT_MOC_LITERAL(16, 182, 16), // "placeToolEnabled"
QT_MOC_LITERAL(17, 199, 12), // "itemSelected"
QT_MOC_LITERAL(18, 212, 13) // "Ref::RefItem*"

    },
    "ObjTools\0enableTool\0\0name\0routeLoaded\0"
    "Route*\0a\0refClassSelected\0text\0"
    "refTrackSelected\0refListSelected\0"
    "QListWidgetItem*\0item\0trackListSelected\0"
    "lastItemsListSelected\0selectToolEnabled\0"
    "placeToolEnabled\0itemSelected\0"
    "Ref::RefItem*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ObjTools[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   67,    2, 0x0a /* Public */,
       7,    1,   70,    2, 0x0a /* Public */,
       9,    1,   73,    2, 0x0a /* Public */,
      10,    1,   76,    2, 0x0a /* Public */,
      13,    1,   79,    2, 0x0a /* Public */,
      14,    1,   82,    2, 0x0a /* Public */,
      15,    0,   85,    2, 0x0a /* Public */,
      16,    0,   86,    2, 0x0a /* Public */,
      17,    1,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   12,

       0        // eod
};

void ObjTools::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ObjTools *_t = static_cast<ObjTools *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enableTool((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->routeLoaded((*reinterpret_cast< Route*(*)>(_a[1]))); break;
        case 2: _t->refClassSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->refTrackSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->refListSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->trackListSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->lastItemsListSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->selectToolEnabled(); break;
        case 8: _t->placeToolEnabled(); break;
        case 9: _t->itemSelected((*reinterpret_cast< Ref::RefItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ObjTools::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ObjTools::enableTool)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ObjTools::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ObjTools.data,
      qt_meta_data_ObjTools,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ObjTools::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ObjTools::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ObjTools.stringdata0))
        return static_cast<void*>(const_cast< ObjTools*>(this));
    return QWidget::qt_metacast(_clname);
}

int ObjTools::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ObjTools::enableTool(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
