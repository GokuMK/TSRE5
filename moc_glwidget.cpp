/****************************************************************************
** Meta object code from reading C++ file 'glwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "glwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GLWidget_t {
    QByteArrayData data[28];
    char stringdata[207];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GLWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GLWidget_t qt_meta_stringdata_GLWidget = {
    {
QT_MOC_LITERAL(0, 0, 8), // "GLWidget"
QT_MOC_LITERAL(1, 9, 11), // "routeLoaded"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 6), // "Route*"
QT_MOC_LITERAL(4, 29, 1), // "a"
QT_MOC_LITERAL(5, 31, 12), // "itemSelected"
QT_MOC_LITERAL(6, 44, 7), // "pointer"
QT_MOC_LITERAL(7, 52, 8), // "naviInfo"
QT_MOC_LITERAL(8, 61, 1), // "x"
QT_MOC_LITERAL(9, 63, 1), // "z"
QT_MOC_LITERAL(10, 65, 3), // "all"
QT_MOC_LITERAL(11, 69, 6), // "hidden"
QT_MOC_LITERAL(12, 76, 10), // "setToolbox"
QT_MOC_LITERAL(13, 87, 4), // "name"
QT_MOC_LITERAL(14, 92, 17), // "setBrushTextureId"
QT_MOC_LITERAL(15, 110, 3), // "val"
QT_MOC_LITERAL(16, 114, 14), // "showProperties"
QT_MOC_LITERAL(17, 129, 9), // "WorldObj*"
QT_MOC_LITERAL(18, 139, 3), // "obj"
QT_MOC_LITERAL(19, 143, 7), // "cleanup"
QT_MOC_LITERAL(20, 151, 10), // "enableTool"
QT_MOC_LITERAL(21, 162, 13), // "setPaintBrush"
QT_MOC_LITERAL(22, 176, 6), // "Brush*"
QT_MOC_LITERAL(23, 183, 5), // "brush"
QT_MOC_LITERAL(24, 189, 6), // "jumpTo"
QT_MOC_LITERAL(25, 196, 1), // "y"
QT_MOC_LITERAL(26, 198, 3), // "msg"
QT_MOC_LITERAL(27, 202, 4) // "text"

    },
    "GLWidget\0routeLoaded\0\0Route*\0a\0"
    "itemSelected\0pointer\0naviInfo\0x\0z\0all\0"
    "hidden\0setToolbox\0name\0setBrushTextureId\0"
    "val\0showProperties\0WorldObj*\0obj\0"
    "cleanup\0enableTool\0setPaintBrush\0"
    "Brush*\0brush\0jumpTo\0y\0msg\0text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GLWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    1,   72,    2, 0x06 /* Public */,
       7,    4,   75,    2, 0x06 /* Public */,
      12,    1,   84,    2, 0x06 /* Public */,
      14,    1,   87,    2, 0x06 /* Public */,
      16,    1,   90,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    0,   93,    2, 0x0a /* Public */,
      20,    1,   94,    2, 0x0a /* Public */,
      21,    1,   97,    2, 0x0a /* Public */,
      24,    2,  100,    2, 0x0a /* Public */,
      26,    1,  105,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    8,    9,   10,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, 0x80000000 | 17,   18,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    8,   25,
    QMetaType::Void, QMetaType::QString,   27,

       0        // eod
};

void GLWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GLWidget *_t = static_cast<GLWidget *>(_o);
        switch (_id) {
        case 0: _t->routeLoaded((*reinterpret_cast< Route*(*)>(_a[1]))); break;
        case 1: _t->itemSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->naviInfo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->setToolbox((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->setBrushTextureId((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->showProperties((*reinterpret_cast< WorldObj*(*)>(_a[1]))); break;
        case 6: _t->cleanup(); break;
        case 7: _t->enableTool((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->setPaintBrush((*reinterpret_cast< Brush*(*)>(_a[1]))); break;
        case 9: _t->jumpTo((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->msg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GLWidget::*_t)(Route * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::routeLoaded)) {
                *result = 0;
            }
        }
        {
            typedef void (GLWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::itemSelected)) {
                *result = 1;
            }
        }
        {
            typedef void (GLWidget::*_t)(int , int , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::naviInfo)) {
                *result = 2;
            }
        }
        {
            typedef void (GLWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::setToolbox)) {
                *result = 3;
            }
        }
        {
            typedef void (GLWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::setBrushTextureId)) {
                *result = 4;
            }
        }
        {
            typedef void (GLWidget::*_t)(WorldObj * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GLWidget::showProperties)) {
                *result = 5;
            }
        }
    }
}

const QMetaObject GLWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_GLWidget.data,
      qt_meta_data_GLWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GLWidget.stringdata))
        return static_cast<void*>(const_cast< GLWidget*>(this));
    if (!strcmp(_clname, "QOpenGLFunctions"))
        return static_cast< QOpenGLFunctions*>(const_cast< GLWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int GLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void GLWidget::routeLoaded(Route * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GLWidget::itemSelected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GLWidget::naviInfo(int _t1, int _t2, int _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GLWidget::setToolbox(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void GLWidget::setBrushTextureId(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void GLWidget::showProperties(WorldObj * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
