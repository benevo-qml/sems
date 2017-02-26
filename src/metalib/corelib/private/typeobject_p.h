/****************************************************************************
**
** Copyright (C) 2015 The oOo Company Ltd.
** Contact: http://www.oOo.io/licensing/
**
** This file is part of the CIM module of the SEMS Toolkit.
**
**  typeobject.h
**  Implementation of the Class TypeObject
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

#ifndef IDENTIFIEDOBJECT_P_H
#define IDENTIFIEDOBJECT_P_H

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

#include "../typeobject.h"

#include <qlist.h>
#include <qscopedpointer.h>
#include <qvariant.h>

CIM_BEGIN_NAMESPACE


class QVariant;

typedef QList<TypeObject *> TestObjPtrLst;

class METALIB_EXPORT TypeObjectPrivate : public TypeObjectData
{
        Q_CIM_DECLARE_PUBLIC(TypeObject)

public:
	struct ObjectData
	{
		ObjectData() {}

		struct  
		{
			QByteArray stringdata;
			QByteArray keys;
		} dd;

		QMap<QByteArray, QVariant> propertyTempData;	// prop name -> prop value

		QString objectName;
	} *objData;

	TypeObjectPrivate(int version = 0);
	virtual ~TypeObjectPrivate();

private:

};

struct AbstractDynamicMetaObject;
struct METALIB_EXPORT DynamicMetaObjectData
{
	virtual ~DynamicMetaObjectData() {}
	virtual void objectDestroyed(TypeObject *) { delete this; }

	virtual AbstractDynamicMetaObject *toDynamicMetaObject(TypeObject *) = 0;
	virtual int metaCall(TypeObject *, MetaObject::Call, int _id, void **) = 0;
};

struct METALIB_EXPORT AbstractDynamicMetaObject : public DynamicMetaObjectData, public MetaObject
{
	virtual AbstractDynamicMetaObject *toDynamicMetaObject(TypeObject *) Q_DECL_OVERRIDE{ return this; }
	virtual int createProperty(const char *, const char *) { return -1; }
	virtual int metaCall(TypeObject *, MetaObject::Call c, int _id, void **a) Q_DECL_OVERRIDE
	{ return metaCall(c, _id, a); }
	virtual int metaCall(MetaObject::Call, int _id, void **) { return _id; } // Compat overload
};

CIM_END_NAMESPACE

#endif // IDENTIFIEDOBJECT_P_H
