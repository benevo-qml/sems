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

#ifndef IDENTIFIEDOBJECT_H
#define IDENTIFIEDOBJECT_H

#include "../metalib_global.h"

#include <qlist.h>
#include <qscopedpointer.h>

#include "typeobjectdefs.h"

CIM_BEGIN_NAMESPACE


class TypeObjectPrivate;
class TypeObject;
struct DynamicMetaObjectData;

typedef QList<TypeObject *> TypeObjectPtrLst;

class TypeObjectData {
public:
    virtual ~TypeObjectData() = 0;
    TypeObject *q_ptr;
    TypeObject *parent;
    TypeObjectPtrLst children;

    uint isPSR : 1;
    uint wasDeleted : 1;
    uint isDeletingChildren : 1;
    uint unused : 25;
    DynamicMetaObjectData *metaObject;
    MetaObject *dynamicMetaObject() const;
};


/**
* This is a root class to provide common information model for all power system classes
* identification and naming attributes. It's purely used to model cim data: properties
*/
class METALIB_EXPORT TypeObject
{
    Q_CIM_OBJECT
    Q_CIM_DECLARE_PRIVATE(TypeObject)

public:
    explicit TypeObject(TypeObject *parent = 0);
    virtual ~TypeObject();

    enum Priority { High, Low, VeryHigh, VeryLow };
    Q_CIM_ENUM(Priority)

    QVariant property(const char *name) const;
    bool setProperty(const char *name, const QVariant &value);

private:
    /**
        * Master resource identifier issued by a model authority. The m_ruid is globally
        * unique within an exchange context.
        * Global uniqeness is easily achived by using a UUID for the m_ruid. It is strongly
        * recommended to do this.
        * For CIMXML data files in RDF syntax, the m_ruid is mapped to rdf:ID or rdf:about
        * attributes that identify CIM object elements.
        */
    Q_CIM_PROPERTY(private, QString, m_ruid, READ, ruid, WRITE, setRuid)

    protected:
        TypeObject(TypeObjectPrivate &dd, TypeObject *parent = 0);

protected:
    //QScopedPointer<TypeObjectData> d_ptr;
    TypeObjectData* d_ptr;

    friend struct MetaObject;

private:
    //Q_DISABLE_COPY(TypeObject)

};

typedef TypeObject* TypeObjectPtr;
Q_DECLARE_METATYPE(TypeObject)
Q_DECLARE_METATYPE(TypeObject*)

CIM_END_NAMESPACE

#endif // IDENTIFIEDOBJECT_H
