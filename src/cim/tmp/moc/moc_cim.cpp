/****************************************************************************
** Meta object code from reading C++ file 'cim.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../cim.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cim.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Cim_t {
    QByteArrayData data[31];
    char stringdata0[357];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Cim_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Cim_t qt_meta_stringdata_Cim = {
    {
QT_MOC_LITERAL(0, 0, 3), // "Cim"
QT_MOC_LITERAL(1, 4, 8), // "CIM_TYPE"
QT_MOC_LITERAL(2, 13, 8), // "TYPE_GEO"
QT_MOC_LITERAL(3, 22, 8), // "TYPE_PSR"
QT_MOC_LITERAL(4, 31, 12), // "CIM_GEO_TYPE"
QT_MOC_LITERAL(5, 44, 9), // "GeoRegion"
QT_MOC_LITERAL(6, 54, 12), // "SubGeoRegion"
QT_MOC_LITERAL(7, 67, 12), // "CIM_PSR_TYPE"
QT_MOC_LITERAL(8, 80, 10), // "Substation"
QT_MOC_LITERAL(9, 91, 5), // "Plant"
QT_MOC_LITERAL(10, 97, 4), // "Line"
QT_MOC_LITERAL(11, 102, 12), // "VoltageLevel"
QT_MOC_LITERAL(12, 115, 3), // "Bay"
QT_MOC_LITERAL(13, 119, 8), // "Terminal"
QT_MOC_LITERAL(14, 128, 13), // "ACLineSegment"
QT_MOC_LITERAL(15, 142, 13), // "DCLineSegment"
QT_MOC_LITERAL(16, 156, 15), // "LoadBreakSwitch"
QT_MOC_LITERAL(17, 172, 8), // "Recloser"
QT_MOC_LITERAL(18, 181, 7), // "Breaker"
QT_MOC_LITERAL(19, 189, 13), // "Sectionaliser"
QT_MOC_LITERAL(20, 203, 6), // "Jumper"
QT_MOC_LITERAL(21, 210, 4), // "Fuse"
QT_MOC_LITERAL(22, 215, 18), // "GroundDisconnector"
QT_MOC_LITERAL(23, 234, 12), // "Disconnector"
QT_MOC_LITERAL(24, 247, 18), // "TransformerWinding"
QT_MOC_LITERAL(25, 266, 16), // "PowerTransformer"
QT_MOC_LITERAL(26, 283, 13), // "BusbarSection"
QT_MOC_LITERAL(27, 297, 8), // "Junction"
QT_MOC_LITERAL(28, 306, 19), // "AsynchronousMachine"
QT_MOC_LITERAL(29, 326, 18), // "SynchronousMachine"
QT_MOC_LITERAL(30, 345, 11) // "PSRTYPE_CNT"

    },
    "Cim\0CIM_TYPE\0TYPE_GEO\0TYPE_PSR\0"
    "CIM_GEO_TYPE\0GeoRegion\0SubGeoRegion\0"
    "CIM_PSR_TYPE\0Substation\0Plant\0Line\0"
    "VoltageLevel\0Bay\0Terminal\0ACLineSegment\0"
    "DCLineSegment\0LoadBreakSwitch\0Recloser\0"
    "Breaker\0Sectionaliser\0Jumper\0Fuse\0"
    "GroundDisconnector\0Disconnector\0"
    "TransformerWinding\0PowerTransformer\0"
    "BusbarSection\0Junction\0AsynchronousMachine\0"
    "SynchronousMachine\0PSRTYPE_CNT"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Cim[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       3,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,    2,   26,
       4, 0x0,    2,   30,
       7, 0x0,   23,   34,

 // enum data: key, value
       2, uint(Cim::TYPE_GEO),
       3, uint(Cim::TYPE_PSR),
       5, uint(Cim::GeoRegion),
       6, uint(Cim::SubGeoRegion),
       8, uint(Cim::Substation),
       9, uint(Cim::Plant),
      10, uint(Cim::Line),
      11, uint(Cim::VoltageLevel),
      12, uint(Cim::Bay),
      13, uint(Cim::Terminal),
      14, uint(Cim::ACLineSegment),
      15, uint(Cim::DCLineSegment),
      16, uint(Cim::LoadBreakSwitch),
      17, uint(Cim::Recloser),
      18, uint(Cim::Breaker),
      19, uint(Cim::Sectionaliser),
      20, uint(Cim::Jumper),
      21, uint(Cim::Fuse),
      22, uint(Cim::GroundDisconnector),
      23, uint(Cim::Disconnector),
      24, uint(Cim::TransformerWinding),
      25, uint(Cim::PowerTransformer),
      26, uint(Cim::BusbarSection),
      27, uint(Cim::Junction),
      28, uint(Cim::AsynchronousMachine),
      29, uint(Cim::SynchronousMachine),
      30, uint(Cim::PSRTYPE_CNT),

       0        // eod
};

void Cim::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject Cim::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Cim.data,
      qt_meta_data_Cim,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Cim::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Cim::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Cim.stringdata0))
        return static_cast<void*>(const_cast< Cim*>(this));
    return QObject::qt_metacast(_clname);
}

int Cim::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
