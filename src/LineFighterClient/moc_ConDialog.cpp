/****************************************************************************
** Meta object code from reading C++ file 'ConDialog.h'
**
** Created: Sat Aug 4 09:37:08 2012
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ConDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_CTcpConDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      24,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CTcpConDialog[] = {
    "CTcpConDialog\0\0ConSig()\0connectToTcp()\0"
};

const QMetaObject CTcpConDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CTcpConDialog,
      qt_meta_data_CTcpConDialog, 0 }
};

const QMetaObject *CTcpConDialog::metaObject() const
{
    return &staticMetaObject;
}

void *CTcpConDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTcpConDialog))
	return static_cast<void*>(const_cast<CTcpConDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CTcpConDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: ConSig(); break;
        case 1: connectToTcp(); break;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CTcpConDialog::ConSig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
