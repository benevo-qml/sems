#ifndef PARSE_GLOBAL_H
#define PARSE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PARSE_LIBRARY)
#  define PARSE_EXPORT Q_DECL_EXPORT
#else
#  define PARSE_EXPORT Q_DECL_IMPORT
#endif

/* user namespace */
#if !defined(PARSE_NAMESPACE)
# define PARSE_USE_NAMESPACE
# define PARSE_BEGIN_NAMESPACE
# define PARSE_END_NAMESPACE
#else
# define PARSE_USE_NAMESPACE
# define PARSE_BEGIN_NAMESPACE
# define PARSE_END_NAMESPACE
#endif

#define oo(str) .append(str)
#define oon(num) .append(QByteArray::number(num))
#define oonlw(num, w) .append(QByteArray::number(num).leftJustified(w - 1, ' '))
#define oonrw(num, w) .append(QByteArray::number(num).rightJustified(w - 1, ' '))

#define ooo(str) .append(str).append('\0')
#define ooon(num) .append(QByteArray::number(num)).append('\0')
#define ooonlw(num, w) .append(QByteArray::number(num).leftJustified(w - 1, ' ')).append('\0')
#define ooonrw(num, w) .append(QByteArray::number(num).rightJustified(w - 1, ' ')).append('\0')

#define LEN_COUNT 4
#define LEN_HANDLE 6
#define LEN_FLAGS 12

#define DELETE_POINTER(p)	{if (NULL != p) {delete p; p = NULL;}}
#define DELETE_POINTERS(p)	{if (NULL != p) {delete[] p; p = NULL;}}

#endif // PARSE_GLOBAL_H
