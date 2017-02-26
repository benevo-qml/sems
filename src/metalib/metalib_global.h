#ifndef METALIB_GLOBAL_H
#define METALIB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QList>

#if defined(METALIB_LIBRARY)
#  define METALIB_EXPORT Q_DECL_EXPORT
#else
#  define METALIB_EXPORT Q_DECL_IMPORT
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

#define CIM_VERSION_STR   "15.33.11.09.09"
/*
CIM_VERSION is (major << 16) + (minor << 8) + patch.
*/
#define CIM_VERSION 0x0F210B0909

#define EPSILON		1E-10f	//浮点数比较阈值
#define LEN_COUNT 4
#define LEN_HANDLE 6
#define LEN_FLAGS 12

#define DELETE_POINTER(p)	{if (NULL != p) {delete p; p = NULL;}}
#define DELETE_POINTERS(p)	{if (NULL != p) {delete[] p; p = NULL;}}

#define oo(str) .append(str)
#define oon(num) .append(QByteArray::number(num))
#define oonlw(num, w) .append(QByteArray::number(num).leftJustified(w - 1, ' '))
#define oonrw(num, w) .append(QByteArray::number(num).rightJustified(w - 1, ' '))
#define ooo(str) .append(str).append('\0')
#define ooon(num) .append(QByteArray::number(num)).append('\0')
#define ooonlw(num, w) .append(QByteArray::number(num).leftJustified(w - 1, ' ')).append('\0')
#define ooonrw(num, w) .append(QByteArray::number(num).rightJustified(w - 1, ' ')).append('\0')

#define Q_CIM_OBJECT \
public: \
    static MetaObject _staticMetaObject; \
    virtual const MetaObject *cim_metaObject() const; \
    virtual void *cim_metacast(const char *); \
    virtual int cim_metacall(MetaObject::Call, int, void **); \
private: \
    static void cim_static_metacall(void *, MetaObject::Call, int, void **);

#define Q_CIM_DECLARE_PRIVATE(Class) \
    inline Class##Private* d_func() { return reinterpret_cast<Class##Private *>(qGetPtrHelper(d_ptr)); } \
    inline const Class##Private* d_func() const { return reinterpret_cast<const Class##Private *>(qGetPtrHelper(d_ptr)); } \
    friend class Class##Private;

#define Q_CIM_DECLARE_PUBLIC(Class)                                    \
    inline Class* q_func() { return static_cast<Class *>(q_ptr); } \
    inline const Class* q_func() const { return static_cast<const Class *>(q_ptr); } \
    friend class Class;

#define Q_CIM_DECLARE_METATYPE(Class) Q_DECLARE_METATYPE(Class)

#define Q_CIM_D(Class) Class##Private * const d = d_func()
#define Q_CIM_Q(Class) Class * const q = q_func()

#define Q_CIM_ENUM(ENUM) \
    friend const MetaObject *cim_getEnumMetaObject(ENUM) { return &_staticMetaObject; } \
    friend const char *cim_getEnumName(ENUM) { return #ENUM; }

#define Q_CIM_PROPERTY(ACCESS, type, name, \
    READ, getter, \
    WRITE, setter \
    )  \
ACCESS: \
    type name; \
    public: \
    type getter() const { return name; } \
    void setter(const type &t) { name = t; }

#define CIM_PTR_PROPERTY(ACCESS, type, name, \
    READ, getter, \
    WRITE, setter \
    )  \
ACCESS: \
    type *name; \
    public: \
    type *getter() { return name; } \
    void setter(type &t) { name = &t; }

#define Q_CIM_PROPERTY_INTCHAR(name, len) \
        char name[len]; \
        int d_##name##() const { return atoi(name); }

#define CIM_PROPERTY2(ACCESS, type, name, \
    READ, type1, getter, \
    WRITE, type2, setter \
    )  \
ACCESS: \
    type name; \
    public: \
    type1 type getter() const { return name; } \
    type2 void setter(const type &t) { name = t; }

#define Q_CIM_DATATYPE(className) \
        Q_CIM_OBJECT \
    public: \
    className(const QByteArray &var); \
    void setValues(const QByteArray &var); \
    QByteArray values() const;

static inline QByteArray nvars(const QByteArray &var, int n) \
{ QByteArray ret; for (int i = 0; i < n; ++i) ret ooo(var); return ret; }

static inline const char *constData_Bi(const QByteArray &bytes, int idx)
{
    const char *p = bytes.constData();
    for (int i = 0; i < idx; ++i)
        p = (p + strlen(p) + 1);

    return p;
}

#endif // METALIB_GLOBAL_H
