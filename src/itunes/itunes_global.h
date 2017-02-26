#ifndef TOOLS_GLOBAL_H
#define TOOLS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ITUNES_LIBRARY)
#  define ITUNES_EXPORT Q_DECL_EXPORT
#else
#  define ITUNES_EXPORT Q_DECL_IMPORT
#endif

#endif // TOOLS_GLOBAL_H
