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

#include "../naming.h"
#include "private/typeobject_p.h"

CIM_BEGIN_NAMESPACE


class NamingPrivate : public TypeObjectPrivate
{
    Q_CIM_DECLARE_PUBLIC(Naming)

public:
    explicit NamingPrivate();

    inline void init();

};


CIM_END_NAMESPACE

#endif // NAMING_P_H
