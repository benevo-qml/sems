#ifndef CIM_GLOBAL_H
#define CIM_GLOBAL_H

#include <QtCore/qglobal.h>
#include "metalib/metalib_global.h"

#if defined(CIM_LIBRARY)
#  define CIM_EXPORT Q_DECL_EXPORT
#else
#  define CIM_EXPORT Q_DECL_IMPORT
#endif

/* user namespace */
#if !defined(CIM_NAMESPACE)
# define CIM_USE_NAMESPACE
# define CIM_BEGIN_NAMESPACE
# define CIM_END_NAMESPACE
#else
# define CIM_USE_NAMESPACE
# define CIM_BEGIN_NAMESPACE
# define CIM_END_NAMESPACE
#endif

#endif // CIM_GLOBAL_H
