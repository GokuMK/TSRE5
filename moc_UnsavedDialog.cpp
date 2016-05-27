/****************************************************************************
** Meta object code from reading C++ file 'UnsavedDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "UnsavedDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'UnsavedDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UnsavedDialog_t {
    QByteArrayData data[5];
    char stringdata[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UnsavedDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UnsavedDialog_t qt_meta_stringdata_UnsavedDialog = {
    {
QT_MOC_LITERAL(0, 0, 13), // "UnsavedDialog"
QT_MOC_LITERAL(1, 14, 2), // "ok"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 6), // "cancel"
QT_MOC_LITERAL(4, 25, 4) // "exit"

    },
    "UnsavedDialog\0ok\0\0cancel\0exit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UnsavedDialog[] = {

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
       3,    0,   30,    2, 0x0a /* Public */,
       4,    0,   31,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UnsavedDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UnsavedDialog *_t = static_cast<UnsavedDialog *>(_o);
        switch (_id) {
        case 0: _t->ok(); break;
        case 1: _t->cancel(); break;
        case 2: _t->exit(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject UnsavedDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_UnsavedDialog.data,
      qt_meta_data_UnsavedDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *UnsavedDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UnsavedDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_UnsavedDialog.stringdata))
        return static_cast<void*>(const_cast< UnsavedDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int UnsavedDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
