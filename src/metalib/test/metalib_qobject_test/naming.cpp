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

#include "Naming.h"
//#include "private/naming_p.h"

//#include "private/naming_moc.h"

//CIM_BEGIN_NAMESPACE

//NamingPrivate::NamingPrivate(int version)
//: TypeObjectPrivate(version)
//{
//}

//NamingPrivate::~NamingPrivate()
//{
//}

/*!
Constructs a push button with no text and a \a parent.
*/

Naming::Naming(QObject *parent)
: QObject(parent)//, TypeObject(*new NamingPrivate, 0)
{
//    Q_D(Naming);
//	d->init();
    CIM_META_REGISTER(Naming) && CIM_META_LOAD(Naming);

	Q_UNUSED(parent);
}

/*! 
	\internal
*/
//Naming::Naming(NamingPrivate &dd, Naming *parent)
//: TypeObject(dd, parent)
//{
//    Q_D(Naming);
//	d->init();
//}

/*!
	Destroys the push button.
*/
Naming::~Naming()
{
}
