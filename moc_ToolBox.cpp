/****************************************************************************
** Meta object code from reading C++ file 'ToolBox.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ToolBox.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ToolBox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ToolBox_t {
    QByteArrayData data[19];
    char stringdata[219];
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
QT_MOC_LITERAL(5, 37, 6), // "Route*"
QT_MOC_LITERAL(6, 44, 1), // "a"
QT_MOC_LITERAL(7, 46, 16), // "refClassSelected"
QT_MOC_LITERAL(8, 63, 4), // "text"
QT_MOC_LITERAL(9, 68, 16), // "refTrackSelected"
QT_MOC_LITERAL(10, 85, 15), // "refListSelected"
QT_MOC_LITERAL(11, 101, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(12, 118, 4), // "item"
QT_MOC_LITERAL(13, 123, 17), // "trackListSelected"
QT_MOC_LITERAL(14, 141, 21), // "lastItemsListSelected"
QT_MOC_LITERAL(15, 163, 17), // "selectToolEnabled"
QT_MOC_LITERAL(16, 181, 16), // "placeToolEnabled"
QT_MOC_LITERAL(17, 198, 12), // "itemSelected"
QT_MOC_LITERAL(18, 211, 7) // "pointer"

    },
    "ToolBox\0enableTool\0\0name\0routeLoaded\0"
    "Route*\0a\0refClassSelected\0text\0"
    "refTrackSelected\0refListSelected\0"
    "QListWidgetItem*\0item\0trackListSelected\0"
    "lastItemsListSelected\0selectToolEnabled\0"
    "placeToolEnabled\0itemSelected\0pointer"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ToolBox[] = {

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
    QMetaType::Void, QMetaType::Int,   18,

       0        // eod
};

void ToolBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ToolBox *_t = static_cast<ToolBox *>(_o);
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
        case 9: _t->itemSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
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
    { &QWidget::staticMetaObject, qt_meta_stringdata_ToolBox.data,
      qt_meta_data_ToolBox,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ToolBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ToolBox::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ToolBox.stringdata))
        return static_cast<void*>(const_cast< ToolBox*>(this));
    return QWidget::qt_metacast(_clname);
}

int ToolBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ToolBox::enableTool(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
