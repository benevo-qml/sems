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

#ifndef METAOBJECT_P_H
#define METAOBJECT_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of moc.  This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//

#include "metalib_global.h"
#include "../metaobject.h"
#include <QtCore/qvarlengtharray.h>

CIM_BEGIN_NAMESPACE

enum PropertyFlags  {
    Invalid = 0x00000000,
    Readable = 0x00000001,
    Writable = 0x00000002,
    Resettable = 0x00000004,
    EnumOrFlag = 0x00000008,
    Visible = 0x00000100,
	Static = 0x00000200,
    Constant = 0x00000400,

    Final = 0x00000800,
    Designable = 0x00001000,
    ResolveDesignable = 0x00002000,
    Scriptable = 0x00004000,
    ResolveScriptable = 0x00008000,
    Stored = 0x00010000,
    ResolveStored = 0x00020000,
    Editable = 0x00040000,
    ResolveEditable = 0x00080000,
    User = 0x00100000,
    ResolveUser = 0x00200000,
    Notify = 0x00400000,
	Revisioned = 0x00800000,

	Pointer = 0x01000000
};

enum MethodFlags  {
    AccessPrivate = 0x00,
    AccessProtected = 0x01,
    AccessPublic = 0x02,
    AccessMask = 0x03, //mask

    MethodMethod = 0x00,
    MethodSignal = 0x04,
    MethodSlot = 0x08,
    MethodConstructor = 0x0c,
    MethodTypeMask = 0x0c,

    MethodCompatibility = 0x10,
    MethodCloned = 0x20,
    MethodScriptable = 0x40,
    MethodRevisioned = 0x80
};

enum MetaObjectFlags {
    DynamicMetaObject = 0x01,
    RequiresVariantMetaObject = 0x02,
    PropertyAccessInStaticMetaCall = 0x04 // since Qt 5.5, property code is in the static metacall
};

enum MetaDataFlags {
	IsUnresolvedType = 0x80000000,
	TypeNameIndexMask = 0x7FFFFFFF
};
extern int qMetaTypeTypeInternal(const char *);

class ArgumentType
{
public:
	ArgumentType(int type)
		: _type(type)
	{}
	ArgumentType(const QByteArray &name)
		: _type(QMetaType::type(name.constData())), _name(name)
	{}
	ArgumentType()
		: _type(0)
	{}
	int type() const
	{
		return _type;
	}
	QByteArray name() const
	{
		if (_type && _name.isEmpty())
			const_cast<ArgumentType *>(this)->_name = QMetaType::typeName(_type);
		return _name;
	}
	bool operator==(const ArgumentType &other) const
	{
		if (_type)
			return _type == other._type;
		else if (other._type)
			return false;
		else
			return _name == other._name;
	}
	bool operator!=(const ArgumentType &other) const
	{
		if (_type)
			return _type != other._type;
		else if (other._type)
			return true;
		else
			return _name != other._name;
	}

private:
	int _type;
	QByteArray _name;
};
typedef QVarLengthArray<ArgumentType, 10> ArgumentTypeArray;

struct MetaObjectPrivate
{
    enum { OutputRevision = 7 }; // Used by moc, qmetaobjectbuilder and qdbus

//	Q_CIM_PROPERTY_INTCHAR(revision, 3);
    char revision[3];
    int d_revision() const { return atoi(revision); }
//	Q_CIM_PROPERTY_INTCHAR(className, LEN_HANDLE);
    char className[LEN_HANDLE];
    int d_className() const { return atoi(className); }
//	Q_CIM_PROPERTY_INTCHAR(classInfoCount, LEN_COUNT);
    char classInfoCount[LEN_COUNT];
    int d_classInfoCount() const { return atoi(classInfoCount); }
//	Q_CIM_PROPERTY_INTCHAR(classInfoData, LEN_HANDLE);
    char classInfoData[LEN_HANDLE];
    int d_classInfoData() const { return atoi(classInfoData); }
//	Q_CIM_PROPERTY_INTCHAR(methodCount, LEN_COUNT);
    char methodCount[LEN_COUNT];
    int d_methodCount() const { return atoi(methodCount); }
//	Q_CIM_PROPERTY_INTCHAR(methodData, LEN_HANDLE);
    char methodData[LEN_HANDLE];
    int d_methodData() const { return atoi(methodData); }
//	Q_CIM_PROPERTY_INTCHAR(propertyCount, LEN_COUNT);
    char propertyCount[LEN_COUNT];
    int d_propertyCount() const { return atoi(propertyCount); }
//	Q_CIM_PROPERTY_INTCHAR(propertyData, LEN_HANDLE);
    char propertyData[LEN_HANDLE];
    int d_propertyData() const { return atoi(propertyData); }
//	Q_CIM_PROPERTY_INTCHAR(enumeratorCount, LEN_COUNT);
    char enumeratorCount[LEN_COUNT];
    int d_enumeratorCount() const { return atoi(enumeratorCount); }
//	Q_CIM_PROPERTY_INTCHAR(enumeratorData, LEN_HANDLE);
    char enumeratorData[LEN_HANDLE];
    int d_enumeratorData() const { return atoi(enumeratorData); }
//	Q_CIM_PROPERTY_INTCHAR(constructorCount, LEN_COUNT);
    char constructorCount[LEN_COUNT];
    int d_constructorCount() const { return atoi(constructorCount); }
//	Q_CIM_PROPERTY_INTCHAR(constructorData, LEN_HANDLE); //since revision 2
    char constructorData[LEN_HANDLE];
    int d_constructorData() const { return atoi(constructorData); }
//	Q_CIM_PROPERTY_INTCHAR(flags, LEN_FLAGS); //since revision 3
    char flags[LEN_FLAGS];
    int d_flags() const { return atoi(flags); }
//	Q_CIM_PROPERTY_INTCHAR(signalCount, LEN_COUNT); //since revision 4
    char signalCount[LEN_COUNT];
    int d_signalCount() const { return atoi(signalCount); }
    // revision 5 introduces changes in normalized signatures, no new members
    // revision 6 added qt_static_metacall as a member of each Q_OBJECT and inside QMetaObject itself
    // revision 7 is Qt 5

	static inline const MetaObjectPrivate *get(const MetaObject *metaobject)
	{
		return reinterpret_cast<const MetaObjectPrivate*>(metaobject->d.keys.data());
	}

	static QByteArray decodeMethodSignature(const char *signature, ArgumentTypeArray &types);
	static int indexOfConstructor(const MetaObject *m, const QByteArray &name, int argc, const ArgumentType *types);

};

// For meta-object generators

enum { MetaObjectPrivateFieldCount = sizeof(MetaObjectPrivate) / sizeof(int) };

#ifndef UTILS_H
// mirrored in moc's utils.h
static inline bool is_ident_char(char s)
{
    return ((s >= 'a' && s <= 'z')
            || (s >= 'A' && s <= 'Z')
            || (s >= '0' && s <= '9')
            || s == '_'
       );
}

static inline bool is_space(char s)
{
    return (s == ' ' || s == '\t');
}
#endif

/*
    This function is shared with moc.cpp. The implementation lives in metaobject_moc_p.h, which
    should be included where needed. The declaration here is not used to avoid warnings from
    the compiler about unused functions.

static QByteArray normalizeTypeInternal(const char *t, const char *e, bool fixScope = false, bool adjustConst = true);
*/

//#pragma init_seg(lib)
//QMap<QByteArray, MetaObject*> g_mapMetaObject;

CIM_END_NAMESPACE

#endif // METAOBJECT_P_H

