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

#ifndef METAOBJECT_H
#define METAOBJECT_H

#include "../metalib_global.h"
#include <QtCore/qvariant.h>

CIM_BEGIN_NAMESPACE


class TypeObject;
struct MetaObject;

class METALIB_EXPORT MetaEnum
{
public:
    inline MetaEnum() : mobj(0),handle(0) {}

    const char *name() const;
    bool isFlag() const;

    int keyCount() const;
    const char *key(int index) const;
    int value(int index) const;

    const char *scope() const;

    int keyToValue(const char *key, bool *ok = 0) const;
    const char* valueToKey(int value) const;
    int keysToValue(const char * keys, bool *ok = 0) const;
    QByteArray valueToKeys(int value) const;

    inline const MetaObject *enclosingMetaObject() const { return mobj; }

    inline bool isValid() const { return name() != 0; }

private:
    const MetaObject *mobj;
    uint handle;
    friend struct MetaObject;
};

class MetaPropertyInfo;
class METALIB_EXPORT MetaProperty
{
public:
    explicit MetaProperty(QObject *parent = 0);

    const char *name() const;
    const char *typeName() const;
	QVariant::Type type() const;
    int userType() const;
    int propertyIndex() const;
	int propertyFlags() const;
	QVariant value() const;

	int propertyInfoCount() const;
	int indexOfPropertyInfo(const char *name) const;
	MetaPropertyInfo propertyInfo(int index) const;

    bool isReadable() const;
    bool isWritable() const;
	bool isResettable() const;
	bool isVisiable() const;
	bool isConstant() const;
	bool isStatic() const;
	bool isPointer() const;

    bool isDesignable(const TypeObject *obj = 0) const;
    bool isScriptable(const TypeObject *obj = 0) const;
    bool isStored(const TypeObject *obj = 0) const;
    bool isEditable(const TypeObject *obj = 0) const;
    bool isUser(const TypeObject *obj = 0) const;
    
    bool isFinal() const;
    bool isFlagType() const;
    bool isEnumType() const;
    MetaEnum enumerator() const;

	int revision() const;

    QVariant read(const TypeObject *obj) const;
    bool write(TypeObject *obj, const QVariant &value) const;
    bool reset(TypeObject *obj) const;

    QVariant readOnGadget(const void *gadget) const;
    bool writeOnGadget(void *gadget, const QVariant &value) const;
    bool resetOnGadget(void *gadget) const;

    bool hasStdCppSet() const;
    inline bool isValid() const { return isReadable(); }
    inline const MetaObject *enclosingMetaObject() const { return mobj; }

	bool setFlag(int flag, bool enabled = true);
	bool setFlags(int flags);
	bool setValue(const QVariant &var);

private:
	int registerPropertyType() const;

    const MetaObject *mobj;
    uint handle;
    int idx;
    MetaEnum menum;
    friend struct MetaObject;
    friend struct MetaObjectPrivate;
};

class METALIB_EXPORT MetaClassInfo
{
public:
    inline MetaClassInfo() : mobj(0),handle(0) {}
    const char *name() const;
    const char *value() const;
    inline const MetaObject *enclosingMetaObject() const { return mobj; }
private:
    const MetaObject *mobj;
    uint handle;
    friend struct MetaObject;
};

class METALIB_EXPORT MetaPropertyInfo
{
public:
	inline MetaPropertyInfo() : mobj(0), handle(0) {}
	const char *name() const;
	const char *value() const;
	inline const MetaObject *enclosingMetaObject() const { return mobj; }
private:
	const MetaObject *mobj;
	uint handle;
	int idx_prop;
	int idx;
	friend struct MetaProperty;
};

CIM_END_NAMESPACE

#endif // METAOBJECT_H
