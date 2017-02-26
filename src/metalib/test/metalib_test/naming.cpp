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

#include "naming.h"
#include "private/naming_p.h"

CIM_BEGIN_NAMESPACE


NamingPrivate::NamingPrivate()
{
    Q_CIM_Q(Naming);
    Q_UNUSED(q);
}

void NamingPrivate::init()
{
    Q_CIM_Q(Naming);
    Q_UNUSED(q);
}

/*!
    Constructs a(n) Naming with a \a parent.
*/
Naming::Naming(Naming *parent)
    :TypeObject(*new NamingPrivate, 0)
{
    Q_CIM_D(Naming);
    d->init();
}

/*!
    \internal
*/
Naming::Naming(NamingPrivate &dd, Naming *parent)
    :TypeObject(dd, 0)
{
    Q_CIM_D(Naming);
    d->init();
}

/*!
    Destroys a(n) Naming.
*/
Naming::~Naming()
{
}


CIM_END_NAMESPACE
