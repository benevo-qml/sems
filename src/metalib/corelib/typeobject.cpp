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

#include "typeobject.h"

#include "metaobject.h"
#include "private/TypeObject_p.h"

#include <QUuid>

CIM_BEGIN_NAMESPACE


TypeObjectData::~TypeObjectData() 
{

}

MetaObject *TypeObjectData::dynamicMetaObject() const
{
    return metaObject->toDynamicMetaObject(q_ptr);
}

TypeObjectPrivate::TypeObjectPrivate(int version)
{
	Q_UNUSED(version);

	// QObjectData initialization
    q_ptr = 0;
	parent = 0;                                 // no parent yet. It is set by setParent()
	isPSR = false;								// assume not a widget object
	wasDeleted = false;                         // double-delete catcher
	isDeletingChildren = false;                 // set by deleteChildren()
	metaObject = 0;
}

TypeObjectPrivate::~TypeObjectPrivate()
{
}


MetaObject TypeObject::_staticMetaObject = {
    Q_NULLPTR, QByteArray("\0"), QByteArray("\0"),
    QByteArray("\0"), cim_static_metacall, Q_NULLPTR, Q_NULLPTR,
    true//CIM_META_REGISTER(TypeObject) && CIM_META_LOAD(TypeObject)
};

/*!
Constructs an object with parent object \a parent.
*/
TypeObject::TypeObject(TypeObject *parent)
: d_ptr(new TypeObjectPrivate)
{
    Q_CIM_D(TypeObject);
    d_ptr->q_ptr = this;

    Q_UNUSED(d);
    Q_UNUSED(parent);

	// Create UUID for the mRID.
	m_ruid = QUuid::createUuid().toString();
    m_ruid.insert(1, QString("CIM_"));
}

/*!
\internal
*/
TypeObject::TypeObject(TypeObjectPrivate &dd, TypeObject *parent)
: d_ptr(&dd)
{
    Q_CIM_D(TypeObject);
    d_ptr->q_ptr = this;

    Q_UNUSED(d);
    Q_UNUSED(parent);

    m_ruid = QUuid::createUuid().toString();
    m_ruid.insert(1, QString("CIM_"));
}

TypeObject::~TypeObject()
{
    Q_CIM_D(TypeObject);
    d->wasDeleted = true;

}


void TypeObject::cim_static_metacall(void *_o, MetaObject::Call _c, int _id, void **_a)
{
	if (_c == MetaObject::CreateInstance) {
		switch (_id) {
		case 0:
		{
			QVariant _r;
			InstanceType &type = *reinterpret_cast<InstanceType*>(_a[1]);
			QByteArray &param = (*reinterpret_cast<QByteArray*>(_a[2]));
			if (HeapInstance == type)
				_r.setValue(new TypeObject);
			else if (StackInstance == type)
				_r.setValue(TypeObject());
			else if (PointerInstance == type)
				_r.setValue(reinterpret_cast<TypeObject*>(param.toULongLong()));

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
            //if (var.canConvert<TypeObject*>())
            //	_r.setValue(var.value<TypeObject*>()->values());
            //else if (var.canConvert<TypeObject>())
            //	_r.setValue(var.value<TypeObject>().values());

			*reinterpret_cast<QVariant*>(_a[0]) = _r;
		}
	}
	else if (_c == MetaObject::ResetProperty) {
	}

	Q_UNUSED(_o);
}

const MetaObject *TypeObject::cim_metaObject() const
{
    return TypeObject::d_ptr->metaObject ? TypeObject::d_ptr->dynamicMetaObject() : &_staticMetaObject;
}

void *TypeObject::cim_metacast(const char *_clname)
{
	if (!_clname) return Q_NULLPTR;
	if (!strcmp(_clname, cim_metaObject()->className()))
		return static_cast<void*>(const_cast<TypeObject*>(this));
	return TypeObject::cim_metacast(_clname);
}

int TypeObject::cim_metacall(MetaObject::Call _c, int _id, void **_a)
{
	if (strcmp(cim_metaObject()->className(), "TypeObject") == 0)
		return  _id = -1;

	_id = TypeObject::cim_metacall(_c, _id, _a);
	if (_id < 0)
		return _id;

#ifndef QT_NO_PROPERTIES
	if (_c == MetaObject::ReadProperty || _c == MetaObject::WriteProperty
		|| _c == MetaObject::ResetProperty || _c == MetaObject::RegisterPropertyMetaType) {
		cim_static_metacall(this, _c, _id, _a);
		_id -= 4;
	}
	else if (_c == MetaObject::QueryPropertyDesignable) {
		_id -= 4;
	}
	else if (_c == MetaObject::QueryPropertyScriptable) {
		_id -= 4;
	}
	else if (_c == MetaObject::QueryPropertyStored) {
		_id -= 4;
	}
	else if (_c == MetaObject::QueryPropertyEditable) {
		_id -= 4;
	}
	else if (_c == MetaObject::QueryPropertyUser) {
		_id -= 4;
	}
#endif // QT_NO_PROPERTIES

	return _id;
}

/*****************************************************************************
Properties
*****************************************************************************/

/*!
	Sets the value of the object's \a name property to \a value.

	If the property is defined in the class using Q_PROPERTY then
	true is returned on success and false otherwise. If the property
	is not defined using Q_PROPERTY, and therefore not listed in the
	meta-object, it is added as a dynamic property and false is returned.

	Information about all available properties is provided through the
	cim_metaObject() and dynamicPropertyNames().

	Dynamic properties can be queried again using property() and can be
	removed by setting the property value to an invalid QVariant.
	Changing the value of a dynamic property causes a QDynamicPropertyChangeEvent
	to be sent to the object.

	\b{Note:} Dynamic properties starting with "_q_" are reserved for internal
	purposes.

	\sa property(), cim_metaObject(), dynamicPropertyNames()
*/
bool TypeObject::setProperty(const char *name, const QVariant &value)
{
    Q_CIM_D(TypeObject);
	const MetaObject* meta = cim_metaObject();
	if (!name || !meta)
		return false;

	int id = meta->indexOfProperty(name);
	if (id < 0) 
	{
        if (!d->objData)
            d->objData = new TypeObjectPrivate::ObjectData;

        bool hasProp = d->objData->propertyTempData.contains(name);

		if (!value.isValid()) 
		{
			if (!hasProp)
				return false;
            d->objData->propertyTempData.remove(name);
		}
		else 
		{
			if (!hasProp)
			{
                d->objData->propertyTempData.insert(name, value);
			}
			else 
			{
                if (value == d->objData->propertyTempData[name])
					return false;
                d->objData->propertyTempData[name] = value;
			}
		}

		//QDynamicPropertyChangeEvent ev(name);
		//QCoreApplication::sendEvent(this, &ev);

		return false;
	}
	MetaProperty p = meta->property(id);
	return p.setValue(value);
	return p.write(this, value);
}

/*!
Returns the value of the object's \a name property.

If no such property exists, the returned variant is invalid.

Information about all available properties is provided through the
cim_metaObject() and dynamicPropertyNames().

\sa setProperty(), QVariant::isValid(), cim_metaObject(), dynamicPropertyNames()
*/
QVariant TypeObject::property(const char *name) const
{
    Q_CIM_D(const TypeObject);
	const MetaObject* meta = cim_metaObject();
	if (!name || !meta)
		return QVariant();

	int id = meta->indexOfProperty(name);
	if (id < 0) 
	{
        if (!d->objData)
			return QVariant();
        return d->objData->propertyTempData[name];
	}
	MetaProperty p = meta->property(id);
	return p.value();
}
