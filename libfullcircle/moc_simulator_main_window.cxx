/****************************************************************************
** Meta object code from reading C++ file 'simulator_main_window.hpp'
**
** Created: Fri Nov 16 22:30:40 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "simulator_main_window.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simulator_main_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fullcircle__SimulatorMainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      42,   33,   32,   32, 0x05,

 // slots: signature, parameters, type, tag, flags
      60,   32,   32,   32, 0x08,
      81,   32,   32,   32, 0x08,
     102,   32,   32,   32, 0x08,
     133,  125,   32,   32, 0x08,
     149,   32,   32,   32, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_fullcircle__SimulatorMainWindow[] = {
    "fullcircle::SimulatorMainWindow\0\0"
    "newValue\0valueChanged(int)\0"
    "on_play_PB_clicked()\0on_stop_PB_clicked()\0"
    "on_reload_PB_clicked()\0frameID\0"
    "draw_frame(int)\0draw_next_frame()\0"
};

void fullcircle::SimulatorMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SimulatorMainWindow *_t = static_cast<SimulatorMainWindow *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_play_PB_clicked(); break;
        case 2: _t->on_stop_PB_clicked(); break;
        case 3: _t->on_reload_PB_clicked(); break;
        case 4: _t->draw_frame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->draw_next_frame(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData fullcircle::SimulatorMainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject fullcircle::SimulatorMainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_fullcircle__SimulatorMainWindow,
      qt_meta_data_fullcircle__SimulatorMainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fullcircle::SimulatorMainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fullcircle::SimulatorMainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fullcircle::SimulatorMainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fullcircle__SimulatorMainWindow))
        return static_cast<void*>(const_cast< SimulatorMainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int fullcircle::SimulatorMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void fullcircle::SimulatorMainWindow::valueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
