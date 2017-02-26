/****************************************************************************
**
** Copyright (C) 2015 The oOo Company Ltd.
** Contact: http://www.oOo.io/licensing/
**
** This file is part of the CIM module of the SEMS Toolkit.
**
**  IdentifiedObject.h
**  Implementation of the Class IdentifiedObject
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

#ifndef NAMING_P_H
#define NAMING_P_H

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

#include "Naming.h"
#include "private/typeobject_p.h"

#include <qlist.h>
#include <qscopedpointer.h>

CIM_BEGIN_NAMESPACE


class QVariant;

class NamingPrivate : public TypeObjectPrivate
{
        Q_DECLARE_PUBLIC(Naming)

public:
        explicit NamingPrivate(int version = 0);
        virtual ~NamingPrivate();

	inline void init() { ; }
};


CIM_END_NAMESPACE

#endif // NAMING_P_H
