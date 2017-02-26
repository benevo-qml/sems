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

#ifndef TEST_OBJECTDEFS_H
#define TEST_OBJECTDEFS_H


#include "../metalib_global.h"
#include <qvariant.h>

CIM_BEGIN_NAMESPACE


class QByteArray;
struct QArrayData;
typedef QArrayData QByteArrayData;

class TypeObject;
class MetaMethod;
class MetaEnum;
class MetaProperty;
class MetaClassInfo;
class SchemaData;


class GenericArgument
{
public:
	inline GenericArgument(const char *aName = 0, const void *aData = 0)
		: _data(aData), _name(aName) {}
	inline void *data() const { return const_cast<void *>(_data); }
	inline const char *name() const { return _name; }

private:
	const void *_data;
	const char *_name;
};

class GenericReturnArgument : public GenericArgument
{
public:
	inline GenericReturnArgument(const char *aName = 0, void *aData = 0)
		: GenericArgument(aName, aData)
	{}
};

template <class T>
class Argument : public GenericArgument
{
public:
	inline Argument(const char *aName, const T &aData)
		: GenericArgument(aName, static_cast<const void *>(&aData))
	{}
};
template <class T>
class Argument<T &> : public GenericArgument
{
public:
	inline Argument(const char *aName, T &aData)
		: GenericArgument(aName, static_cast<const void *>(&aData))
	{}
};


template <typename T>
class ReturnArgument : public GenericReturnArgument
{
public:
	inline ReturnArgument(const char *aName, T &aData)
		: GenericReturnArgument(aName, static_cast<void *>(&aData))
	{}
};

enum InstanceType  {
	HeapInstance,		// in heap mem, return pointer
	StackInstance,		// in stack mem, return real instance
	PointerInstance
};

struct METALIB_EXPORT MetaObject
{
	const char *className() const;
	const MetaObject *superClass() const;

	TypeObject *cast(TypeObject *obj) const;
	const TypeObject *cast(const TypeObject *obj) const;

	int propertyOffset() const;
	int classInfoOffset() const;
	
	int propertyCount() const;
	int classInfoCount() const;

	int indexOfProperty(const char *name) const;
	int indexOfClassInfo(const char *name) const;

	MetaProperty property(int index) const;
	MetaClassInfo classInfo(int index) const;

	int objDataCount() const;

	int indexOfConstructor(const char *constructor) const;
	void print(const char *dirPath) const;

	QVariant createInstance(InstanceType type = HeapInstance,
        QByteArray val0 = QByteArray(),
        QByteArray val1 = QByteArray(),
        QByteArray val2 = QByteArray(),
        QByteArray val3 = QByteArray(),
        QByteArray val4 = QByteArray(),
        QByteArray val5 = QByteArray(),
        QByteArray val6 = QByteArray(),
        QByteArray val7 = QByteArray(),
        QByteArray val8 = QByteArray(),
        QByteArray val9 = QByteArray()) const;
	QVariant retrieveObjData(QVariant &varobj) const;

	static QByteArray normalizedSignature(const char *method);
    static QVariant createTypeInstance(const char *name,
        QByteArray val0 = QByteArray(),
        QByteArray val1 = QByteArray(),
        QByteArray val2 = QByteArray(),
        QByteArray val3 = QByteArray(),
        QByteArray val4 = QByteArray(),
        QByteArray val5 = QByteArray(),
        QByteArray val6 = QByteArray(),
        QByteArray val7 = QByteArray(),
        QByteArray val8 = QByteArray(),
        QByteArray val9 = QByteArray());

	enum Call {
		InvokeMetaMethod,
		ReadProperty,
		WriteProperty,
		ResetProperty,
		QueryPropertyDesignable,
		QueryPropertyScriptable,
		QueryPropertyStored,
		QueryPropertyEditable,
		QueryPropertyUser,
		CreateInstance,
		RetrieveObjData,
		IndexOfMethod,
		RegisterPropertyMetaType,
		RegisterMethodArgumentMetaType
	};

	int static_metacall(Call, int, void **) const;
	static int metacall(TypeObject *, Call, int, void **);

	struct { // private data
		const MetaObject *superdata;
		QByteArray stringdata;
		QByteArray keys;
		QByteArray objdata;
		typedef void(*StaticMetacallFunction)(void *, MetaObject::Call, int, void **);
		StaticMetacallFunction static_metacall;
		const MetaObject * const *relatedMetaObjects;
		void *extradata; //reserved for future use
		bool userTypeReg;
	} d;

    //~MetaObject();
};

inline const MetaObject *MetaObject::superClass() const
{ return d.superdata; }

extern QByteArrayData *GenerateOffsetKey(char *text, int nTextLen);
extern METALIB_EXPORT MetaObject *qMapMetaObject(QByteArray className);
extern METALIB_EXPORT bool qLoadAllMetaObjects();
extern METALIB_EXPORT bool qLoadMetaObject(const char *className, MetaObject *mobj = 0);
extern METALIB_EXPORT bool qRegisterMetaObject(QByteArray className, MetaObject *mobj);
class QSqlDatabase;
extern QSqlDatabase METALIB_EXPORT qSqliteDatabase(const QString &path_db);

#define CIM_META_REGISTER(CLASS) \
	qRegisterMetaObject(#CLASS, &CLASS::_staticMetaObject)
#define CIM_META_LOAD(CLASS) \
    qLoadMetaObject(#CLASS, &CLASS::_staticMetaObject)

CIM_END_NAMESPACE

#endif // TEST_OBJECTDEFS_H
