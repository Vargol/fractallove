/****************************************************************************
** Meta object code from reading C++ file 'FractalController.h'
**
** Created: Wed Mar 11 21:35:01 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../FractalController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FractalController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FractalController[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   35,   18,   18, 0x0a,
      79,   73,   18,   18, 0x0a,
     100,   73,   18,   18, 0x0a,
     122,   73,   18,   18, 0x0a,
     142,  138,   18,   18, 0x0a,
     169,  167,   18,   18, 0x0a,
     190,  188,   18,   18, 0x0a,
     209,   18,   18,   18, 0x0a,
     226,   18,   18,   18, 0x0a,
     257,  244,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FractalController[] = {
    "FractalController\0\0redrawFractal()\0"
    "width,height\0setViewPortSize(int,int)\0"
    "value\0setFractalWidth(int)\0"
    "setFractalHeight(int)\0setZoom(double)\0"
    "x,y\0setOffset(double,double)\0x\0"
    "setXOffset(double)\0y\0setYOffset(double)\0"
    "renderToScreen()\0renderToTexture()\0"
    "fractalClass\0setFractalClass(QString)\0"
};

const QMetaObject FractalController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FractalController,
      qt_meta_data_FractalController, 0 }
};

const QMetaObject *FractalController::metaObject() const
{
    return &staticMetaObject;
}

void *FractalController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FractalController))
        return static_cast<void*>(const_cast< FractalController*>(this));
    return QObject::qt_metacast(_clname);
}

int FractalController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: redrawFractal(); break;
        case 1: setViewPortSize((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: setFractalWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: setFractalHeight((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: setZoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: setOffset((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: setXOffset((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: setYOffset((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: renderToScreen(); break;
        case 9: renderToTexture(); break;
        case 10: setFractalClass((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void FractalController::redrawFractal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
