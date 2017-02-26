/****************************************************************************
**
** Copyright (C) 2015 The oOo Company Ltd.
** Contact: http://www.oOo.io/licensing/
**
** This file is part of the CIM module of the SEMS Toolkit.
**
**  IdentifiedObject.h
**  Implementation of the Class IdentifiedObject
**  Created on:      13-Nov-2015 13:15:35
**  Original author: benev
**
** $CIM_BEGIN_LICENSE:LGPL21$
**
** LICENSE INFO ABOUT ADD HERE...
**
** $CIM_END_LICENSE$
**
****************************************************************************/

#ifndef NAMING_MOC_H
#define NAMING_MOC_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the SEMS API.  It exists for the convenience
// of _*.cpp.  This header file may change from version to version 
// without notice, or even be removed.
//
// We mean it.
//

#include "Naming.h"
#include "private/typeobject_p.h"

#include <qlist.h>
#include <qscopedpointer.h>

CIM_BEGIN_NAMESPACE


MetaObject Naming::_staticMetaObject = {
        Q_NULLPTR, QByteArray("\0"), QByteArray("\0")
        , QByteArray("\0"), cim_static_metacall, Q_NULLPTR, Q_NULLPTR,
    CIM_META_REGISTER(Naming) && CIM_META_LOAD(Naming)
};

const MetaObject *Naming::cim_metaObject() const
{
        return &_staticMetaObject;
}

void *Naming::cim_metacast(const char *_clname)
{
        if (!_clname) return Q_NULLPTR;
        if (!strcmp(_clname, cim_metaObject()->className()))
                return static_cast<void*>(const_cast<Naming*>(this));
        return Naming::cim_metacast(_clname);
}

int Naming::cim_metacall(MetaObject::Call _c, int _id, void **_a)
{
        if (strcmp(cim_metaObject()->className(), "Naming") == 0)
                return  _id = -1;

        _id = Naming::cim_metacall(_c, _id, _a);
        if (_id < 0)
                return _id;

        return _id;
}

void Naming::cim_static_metacall(void *_o, MetaObject::Call _c, int _id, void **_a)
{
        if (_c == MetaObject::CreateInstance) {
                switch (_id) {
                case 0:
                {
                        QVariant _r;
                        InstanceType &type = *reinterpret_cast<InstanceType*>(_a[1]);
                        QByteArray &param = (*reinterpret_cast<QByteArray*>(_a[2]));
                        if (HeapInstance == type)
                                _r.setValue(new Naming());
                        else if (StackInstance == type)
                                _r.setValue(Naming());
                        else if (PointerInstance == type)
                                _r.setValue(reinterpret_cast<Naming*>(param.toULongLong()));

                        *reinterpret_cast<QVariant*>(_a[0]) = _r;
                } break;
                default: break;
                }
        }
        else if (_c == MetaObject::RetrieveObjData) {
                if (_a[0])
                {
                        QVariant &var = (*reinterpret_cast<QVariant*>(_a[1]));
                        QVariant _r;
//                        if (var.canConvert<Naming*>())
//                                _r.setValue(var.value<Naming*>()->values());
//                        else if (var.canConvert<Naming>())
//                                _r.setValue(var.value<Naming>().values());

                        *reinterpret_cast<QVariant*>(_a[0]) = _r;
                }
        }

        Q_UNUSED(_o);
}



CIM_END_NAMESPACE

#endif // NAMING_MOC_H
