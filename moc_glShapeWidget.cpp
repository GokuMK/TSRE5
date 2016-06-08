/****************************************************************************
** Meta object code from reading C++ file 'glShapeWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "glShapeWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glShapeWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GlShapeWidget_t {
    QByteArrayData data[11];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GlShapeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GlShapeWidget_t qt_meta_stringdata_GlShapeWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GlShapeWidget"
QT_MOC_LITERAL(1, 14, 8), // "selected"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 2), // "id"
QT_MOC_LITERAL(4, 27, 11), // "refreshItem"
QT_MOC_LITERAL(5, 39, 7), // "showEng"
QT_MOC_LITERAL(6, 47, 4), // "path"
QT_MOC_LITERAL(7, 52, 4), // "name"
QT_MOC_LITERAL(8, 57, 7), // "showCon"
QT_MOC_LITERAL(9, 65, 3), // "aid"
QT_MOC_LITERAL(10, 69, 7) // "cleanup"

    },
    "GlShapeWidget\0selected\0\0id\0refreshItem\0"
    "showEng\0path\0name\0showCon\0aid\0cleanup"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GlShapeWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    0,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    2,   48,    2, 0x0a /* Public */,
       8,    1,   53,    2, 0x0a /* Public */,
       8,    2,   56,    2, 0x0a /* Public */,
      10,    0,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    9,    3,
    QMetaType::Void,

       0        // eod
};

void GlShapeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GlShapeWidget *_t = static_cast<GlShapeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->refreshItem(); break;
        case 2: _t->showEng((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->showCon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->showCon((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->cleanup(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GlShapeWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GlShapeWidget::selected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (GlShapeWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GlShapeWidget::refreshItem)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject GlShapeWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_GlShapeWidget.data,
      qt_meta_data_GlShapeWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GlShapeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlShapeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GlShapeWidget.stringdata0))
        return static_cast<void*>(const_cast< GlShapeWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(const_cast< GlShapeWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int GlShapeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
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
void GlShapeWidget::selected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GlShapeWidget::refreshItem()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
