/****************************************************************************
** Meta object code from reading C++ file 'GamingWidget.h'
**
** Created: Mon Aug 6 22:35:30 2012
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "GamingWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GamingWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_CGamingWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CGamingWidget[] = {
    "CGamingWidget\0\0changeStatus(int)\0"
};

const QMetaObject CGamingWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CGamingWidget,
      qt_meta_data_CGamingWidget, 0 }
};

const QMetaObject *CGamingWidget::metaObject() const
{
    return &staticMetaObject;
}

void *CGamingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CGamingWidget))
	return static_cast<void*>(const_cast<CGamingWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CGamingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CGamingWidget::changeStatus(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
