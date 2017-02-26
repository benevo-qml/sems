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

#include <qlist.h>
#include <qscopedpointer.h>

#include "typeobject.h"

CIM_BEGIN_NAMESPACE


class NamingPrivate;

/**
 * This is a root class to provide common naming attributes for all classes needing naming attributes
 */
class Naming : public TypeObject
{
    Q_CIM_OBJECT

    /**
    * Free text name of the object or instance.
    */
    Q_CIM_PROPERTY(private, QString, m_aliasName, READ, aliasName, WRITE, setAliasName)
    /**
    * Description of the object or instance.
    */
    Q_CIM_PROPERTY(private, QString, m_description, READ, description, WRITE, setDescription)
    /**
    * Unique name among objects owned by the same parent.
    */
    Q_CIM_PROPERTY(private, QString, m_name, READ, name, WRITE, setName)
    /**
    * pathName is a concatenation of all names from each container.
    */
    Q_CIM_PROPERTY(private, QString, m_pathName, READ, pathName, WRITE, setPathName)

public:
    explicit Naming(Naming *parent = 0);
    virtual ~Naming();

protected:
    Naming(NamingPrivate &dd, Naming *parent = 0);

private:
    Q_CIM_DECLARE_PRIVATE(Naming)

};

Q_CIM_DECLARE_METATYPE(Naming)
Q_CIM_DECLARE_METATYPE(Naming*)

typedef Naming* NamingPtr;


CIM_END_NAMESPACE

#endif // NAMING_H
