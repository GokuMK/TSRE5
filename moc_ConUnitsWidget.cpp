/****************************************************************************
** Meta object code from reading C++ file 'ConUnitsWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ConUnitsWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConUnitsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ConUnitsWidget_t {
    QByteArrayData data[12];
    char stringdata0[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConUnitsWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConUnitsWidget_t qt_meta_stringdata_ConUnitsWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ConUnitsWidget"
QT_MOC_LITERAL(1, 15, 8), // "selected"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 2), // "id"
QT_MOC_LITERAL(4, 28, 11), // "refreshItem"
QT_MOC_LITERAL(5, 40, 13), // "itemsSelected"
QT_MOC_LITERAL(6, 54, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(7, 71, 2), // "it"
QT_MOC_LITERAL(8, 74, 12), // "bDelReleased"
QT_MOC_LITERAL(9, 87, 11), // "bUpReleased"
QT_MOC_LITERAL(10, 99, 13), // "bDownReleased"
QT_MOC_LITERAL(11, 113, 13) // "bFlipReleased"

    },
    "ConUnitsWidget\0selected\0\0id\0refreshItem\0"
    "itemsSelected\0QListWidgetItem*\0it\0"
    "bDelReleased\0bUpReleased\0bDownReleased\0"
    "bFlipReleased"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConUnitsWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    0,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   53,    2, 0x0a /* Public */,
       8,    0,   56,    2, 0x0a /* Public */,
       9,    0,   57,    2, 0x0a /* Public */,
      10,    0,   58,    2, 0x0a /* Public */,
      11,    0,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConUnitsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ConUnitsWidget *_t = static_cast<ConUnitsWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->refreshItem(); break;
        case 2: _t->itemsSelected((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->bDelReleased(); break;
        case 4: _t->bUpReleased(); break;
        case 5: _t->bDownReleased(); break;
        case 6: _t->bFlipReleased(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ConUnitsWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConUnitsWidget::selected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (ConUnitsWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ConUnitsWidget::refreshItem)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject ConUnitsWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConUnitsWidget.data,
      qt_meta_data_ConUnitsWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ConUnitsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConUnitsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ConUnitsWidget.stringdata0))
        return static_cast<void*>(const_cast< ConUnitsWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConUnitsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void ConUnitsWidget::selected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ConUnitsWidget::refreshItem()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
