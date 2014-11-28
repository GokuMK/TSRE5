/****************************************************************************
** Meta object code from reading C++ file 'ToolBox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ToolBox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ToolBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ToolBox_t {
    QByteArrayData data[14];
    char stringdata[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ToolBox_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ToolBox_t qt_meta_stringdata_ToolBox = {
    {
QT_MOC_LITERAL(0, 0, 7), // "ToolBox"
QT_MOC_LITERAL(1, 8, 10), // "enableTool"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 4), // "name"
QT_MOC_LITERAL(4, 25, 11), // "routeLoaded"
QT_MOC_LITERAL(5, 37, 4), // "Ref*"
QT_MOC_LITERAL(6, 42, 1), // "a"
QT_MOC_LITERAL(7, 44, 16), // "refClassSelected"
QT_MOC_LITERAL(8, 61, 4), // "text"
QT_MOC_LITERAL(9, 66, 15), // "refListSelected"
QT_MOC_LITERAL(10, 82, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(11, 99, 4), // "item"
QT_MOC_LITERAL(12, 104, 17), // "selectToolEnabled"
QT_MOC_LITERAL(13, 122, 16) // "placeToolEnabled"

    },
    "ToolBox\0enableTool\0\0name\0routeLoaded\0"
    "Ref*\0a\0refClassSelected\0text\0"
    "refListSelected\0QListWidgetItem*\0item\0"
    "selectToolEnabled\0placeToolEnabled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolBox[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   47,    2, 0x0a /* Public */,
       7,    1,   50,    2, 0x0a /* Public */,
       9,    1,   53,    2, 0x0a /* Public */,
      12,    0,   56,    2, 0x0a /* Public */,
      13,    0,   57,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ToolBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ToolBox *_t = static_cast<ToolBox *>(_o);
        switch (_id) {
        case 0: _t->enableTool((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->routeLoaded((*reinterpret_cast< Ref*(*)>(_a[1]))); break;
        case 2: _t->refClassSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->refListSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->selectToolEnabled(); break;
        case 5: _t->placeToolEnabled(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ToolBox::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ToolBox::enableTool)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject ToolBox::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_ToolBox.data,
      qt_meta_data_ToolBox,  qt_static_metacall, 0, 0}
};


const QMetaObject *ToolBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ToolBox.stringdata))
        return static_cast<void*>(const_cast< ToolBox*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int ToolBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void ToolBox::enableTool(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
