/****************************************************************************
**
** Copyright (C) 2015 The oOo Company Ltd.
** Contact: http://www.oOo.io/licensing/
**
** This file is part of the CIM module of the SEMS Toolkit.
**
** $CIM_BEGIN_LICENSE:LGPL21$
**
** LICENSE INFO ABOUT ADD HERE...
**
** $CIM_END_LICENSE$
**
****************************************************************************/

#ifndef NAMING_H
#define NAMING_H

#include <QObject>

#include "typeobject.h"

CIM_BEGIN_NAMESPACE


class NamingPrivate;
class PSRType;
class Measurement;

typedef QList<Measurement *> MeasurementPtrList;

/**
 * A power system resource can be an item of equipment such as a switch, an
 * equipment container containing many individual items of equipment such as a
 * substation, or an organisational entity such as sub-control area. Power system
 * resources can have measurements associated.
 */
class Naming : public QObject, public TypeObject
{
//    CIM_OBJECT
    Q_OBJECT

public:
    explicit Naming(QObject *parent = 0);
    virtual ~Naming();

//    virtual bool Group(TypeObject *obj)			{ Q_UNUSED(obj);  return true; }
//    virtual bool UnGroup(TypeObject *obj)			{ Q_UNUSED(obj);  return true; }
//    virtual const char* GetPsrTypeName()					{ return NULL; }

protected:
//    Naming(NamingPrivate &dd, Naming* parent = 0);

public:

private:
//    Q_DISABLE_COPY(Naming)
//    Q_DECLARE_PRIVATE(Naming)
};

typedef QList<Naming *> PSRList;

//Q_DECLARE_METATYPE(Naming)
//Q_DECLARE_METATYPE(Naming*)

CIM_END_NAMESPACE

#endif // NAMING_H
