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

#include "metaobject.h"

#include <qfile.h>
#include <qdir.h>
#include <qcoreapplication.h>
#include <qcoreevent.h>
#include <qdatastream.h>
#include <qstringlist.h>
#include <qthread.h>
#include <qvariant.h>
#include <qhash.h>
#include <qdebug.h>
#include <qsemaphore.h>
#include <qdatetime.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <qsqlerror.h>
#include <ctype.h>

#include "private/typeobject_p.h"
#include "private/metaobject_p.h"
#include "typeobject.h"
#include "metalib.h"

#include <iostream>
using namespace std;

CIM_BEGIN_NAMESPACE

#define LEN_PROPERTY_FIELD 7

#define META_OBJECT_MOC_LITERAL(ofs0, idx, ofs, len) \
{ (-1), len, 0, 0, \
    qptrdiff(ofs0 + ofs - idx * sizeof(QByteArrayData)) \
}

#define KEY_DATA(m, idx) atoi(constData_Bi(m->d.keys, idx))

//#pragma init_seg(lib)
QMap<QByteArray, MetaObject*> g_mapMetaObject;

/*!
    priv将d.data解释为MetaObjectPrivate。
*/

MetaObject *qMapMetaObject(QByteArray className)
{
    className = className.trimmed();
    if (className.length() > 0 && className[className.length() - 1] == '*')
        className.chop(1);
    if (g_mapMetaObject.contains(className.trimmed()))
        return g_mapMetaObject[className.trimmed()];
    else
        return NULL;
}

bool qRegisterMetaObject(QByteArray className, MetaObject *mobj)
{
    if (g_mapMetaObject.contains(className))
        return false;

    g_mapMetaObject.insert(className, mobj);

    return true;
}

QSqlDatabase qSqliteDatabase(const QString &path_db)
{
    QSqlDatabase pdb;
    if (path_db.isEmpty())
        return pdb;
    QString connName = "iYoona.db.meta";
    if (QSqlDatabase::contains(connName))
        pdb = QSqlDatabase::database(connName);
    else
        pdb = QSqlDatabase::addDatabase("QSQLITE", connName);
    Q_ASSERT(pdb.isValid());
    pdb.setDatabaseName(path_db);

    if (!pdb.open())
        return pdb;

    pdb.close();

    return pdb;
}

bool qLoadMetaObject(const char *className, MetaObject *mobj)
{
    if (NULL == className || strcmp(className, "") == 0)
        return false;

    QSqlDatabase pdb = qSqliteDatabase(Metalib::applicationDirPath() + "/../data/cim/metalib.db");
    Q_ASSERT(pdb.isValid());
    if (!pdb.open()){
        qDebug() << "QSqlite open failed, wrong db path: " << pdb.databaseName();
        return false;
    }

    QSqlQuery query(pdb);
    query.prepare("SELECT name, stringdata,key, objdata, supper FROM tb_metaobject WHERE lower(name) = ?");
    query.addBindValue(QByteArray(className).toLower().data());
    if (query.exec() && query.next())
    {
        QByteArray ele0 = query.value(0).toByteArray();
        QByteArray ele1 = query.value(1).toByteArray();
        QByteArray ele2 = query.value(2).toByteArray();
        QByteArray ele3 = query.value(3).toByteArray();
        QByteArray ele4 = query.value(4).toByteArray();

        if (0 == mobj)
            mobj = qMapMetaObject(ele0);
        qRegisterMetaObject(ele0, mobj);
        MetaObject &mo = *mobj;
        mo.d.stringdata = ele1;
        mo.d.keys = ele2;
        mo.d.objdata = ele3;
    }
    else
    {
        qDebug() << "show last query is : " << qPrintable(query.lastQuery());
        qDebug() << "show last error is : " << qPrintable(query.lastError().text());

        query.finish();
        return false;
    }
    query.finish();

    return true;
}

bool qLoadAllMetaObjects()
{
    if (g_mapMetaObject.isEmpty())
        return false;

    QSqlDatabase pdb = qSqliteDatabase(Metalib::applicationDirPath() + "/../data/cim/metalib.db");
    if (!pdb.open()){
        qDebug() << "QSqlite open failed, wrong db path: " << pdb.databaseName();
        return false;
    }
    QSqlQuery query(pdb);

    QMapIterator<QByteArray, MetaObject*> i(g_mapMetaObject);
    while (i.hasNext())
    {
        i.next();

        MetaObject *pmo = i.value();

        if (NULL == pmo)
            continue;

        query.prepare("SELECT name, stringdata,key, objdata, supper FROM tb_metaobject WHERE lower(name) = ?");
        query.addBindValue(i.key().toLower().data());
        if (query.exec() && query.next())
        {
            QByteArray ele0 = query.value(0).toByteArray();
            QByteArray ele1 = query.value(1).toByteArray();
            QByteArray ele2 = query.value(2).toByteArray();
            QByteArray ele3 = query.value(3).toByteArray();
            QByteArray ele4 = query.value(4).toByteArray();

            MetaObject &mo = *pmo;
            mo.d.stringdata = ele1;
            mo.d.keys = ele2;
            mo.d.objdata = ele3;
        }
        else
        {
            qDebug() << "show last query is : " << qPrintable(query.lastQuery());
            qDebug() << "show last error is : " << qPrintable(query.lastError().text());
            query.finish();
            return false;
        }
        query.finish();
    }

    //qRegisterMetaObject(className, &TypeObject::_staticMetaObject);

    return true;
}

static inline const MetaObjectPrivate *priv(const QByteArray &d)
{
    return reinterpret_cast<const MetaObjectPrivate*>(d.data());
}

static inline char *data_Bi(QByteArray &bytes, int idx, int &offset)
{
    offset = 0;
    char *p = bytes.data();
    for (int i = 0; i < idx; ++i)
    {
        offset += strlen(p) + 1;
        p = (p + strlen(p) + 1);
    }

    return p;
}

static inline QByteArray mid_Bij(const QByteArray &bytes, int start, int end = -1)
{
    if (-1 != end && end < start)
        return QByteArray();

    int nCount = bytes.count('\0');
    if (start > nCount - 1
        || end > nCount - 1)
        return QByteArray();

    int pos0 = 0;
    int pos1 = 0;
    int idx = qMax(start, end);
    for (int i = 0, from = 0; i <= idx; ++i)
    {
        from = bytes.indexOf('\0', from) + 1;
        if (i == start - 1)
            pos0 = from;
        if (i == end)
        {
            pos1 = from;
            break;
        }
    }

    return bytes.mid(pos0, end < 0 ? -1 : pos1 - pos0);
}

static inline QByteArray right_Bij(const QByteArray &bytes, int n)
{
    if (n < 1)
        return QByteArray();

    int len = 0;
    int from = -1;
    for (int i = 0; i <= n; ++i)
    {
        from = bytes.lastIndexOf('\0', from) - 1;
    }
    len = bytes.length() - from - 2;

    return bytes.right(len);
}

static inline QByteArray left_ByteArray(const QByteArray &bytes, int n)
{
    if (n < 1)
        return QByteArray();

    int from = 0;
    for (int i = 0; i < n; ++i)
    {
        from = bytes.indexOf('\0', from) + 1;
    }

    return bytes.left(from);
}

bool setData_Bij(QByteArray &bytes, int start, int end, const QByteArray &_newBytes)
{
    if (-1 != end && end < start)
        return false;

    QByteArray newBytes = _newBytes;
    if (newBytes.lastIndexOf('\0') != newBytes.length() - 1)
        newBytes.append('\0');

    int nCount = bytes.count('\0');
    if (start > nCount - 1
        || end > nCount - 1)
        return false;

    int offset0 = 0;
    char *p = data_Bi(bytes, start, offset0);
    int offset1 = 0;
    if (end == -1)
        offset1 = bytes.length();
    else
        data_Bi(bytes, end + 1, offset1);
    int oldLen = offset1 - offset0 - 1;
    //qDebug() << "equals? " << (oldLen == mid_Bij(bytes, start, end).length());

    int newLen = newBytes.length() - 1;
    if (newLen < oldLen)
        bytes.remove(offset0, oldLen - newLen);
    else if (newLen > oldLen)
        bytes.insert(offset0, QByteArray(" ").leftJustified(newLen - oldLen));

    p = data_Bi(bytes, start, offset0); // recalc
    for (int i = 0; i < newLen; ++i)
    {
        *(p + i) = newBytes[i];
    }

    return true;
}

static inline const char *rawStringData(const MetaObject *mo, int index)
{
    return constData_Bi(mo->d.stringdata, index);
}

static inline const QByteArray rawObjData(const MetaObject *mo, int index_start, int index_end)
{
    return mid_Bij(mo->d.objdata, index_start, index_end);
}

const char *typeName(int typeId)
{
    const uint type = typeId;
    // In theory it can be filled during compilation time, but for some reason template code
    // that is able to do it causes GCC 4.6 to generate additional 3K of executable code. Probably
    // it is not worth of it.
    static const char *namesCache[QMetaType::HighestInternalId + 1];

    const char *result;
    if (type <= QMetaType::HighestInternalId && ((result = namesCache[type])))
        return result;

#define QT_METATYPE_TYPEID_TYPENAME_CONVERTER(MetaTypeName, TypeId, RealName) \
        case QMetaType::MetaTypeName: result = #RealName; break;

    switch (QMetaType::Type(type)) {
        QT_FOR_EACH_STATIC_TYPE(QT_METATYPE_TYPEID_TYPENAME_CONVERTER)

    default: {
            if (Q_UNLIKELY(type < QMetaType::User)) {
                return 0; // It can happen when someone cast int to QVariant::Type, we should not crash...
            }
            else {
                //const QVector<QCustomTypeInfo> * const ct = customTypes();
                //QReadLocker locker(customTypesLock());
                //return ct && uint(ct->count()) > type - QMetaType::User && !ct->at(type - QMetaType::User).typeName.isEmpty()
                //	? ct->at(type - QMetaType::User).typeName.constData()
                //	: 0;
            }
        }
    }
#undef QT_METATYPE_TYPEID_TYPENAME_CONVERTER

    Q_ASSERT(type <= QMetaType::HighestInternalId);
    namesCache[type] = result;
    return result;
}

static inline QByteArray typeNameFromTypeInfo(const MetaObject *mo, uint typeInfo)
{
    if (typeInfo & IsUnresolvedType) {
        return rawStringData(mo, typeInfo & TypeNameIndexMask);
    } else {
        // ### Use the QMetaType::typeName() that returns QByteArray
        const char *t = /*QMetaType::*/typeName(typeInfo);
        return QByteArray::fromRawData(t, qstrlen(t));
    }
}

static inline const char *rawTypeNameFromTypeInfo(const MetaObject *mo, uint typeInfo)
{
    return typeNameFromTypeInfo(mo, typeInfo).constData();
}

static inline int typeFromTypeInfo(const MetaObject *mo, uint typeInfo)
{
    if (!(typeInfo & IsUnresolvedType))
        return typeInfo;
    return QMetaType::type(rawStringData(mo, typeInfo & TypeNameIndexMask));
}

// Parses a string of comma-separated types into QArgumentTypes.
// No normalization of the type names is performed.
static void argumentTypesFromString(const char *str, const char *end,
    ArgumentTypeArray &types)
{
    Q_ASSERT(str <= end);
    while (str != end) {
        if (!types.isEmpty())
            ++str; // Skip comma
        const char *begin = str;
        int level = 0;
        while (str != end && (level > 0 || *str != ',')) {
            if (*str == '<')
                ++level;
            else if (*str == '>')
                --level;
            ++str;
        }
        types += ArgumentType(QByteArray(begin, str - begin));
    }
}

// Returns \c true if the method defined by the given meta-object&handle
// matches the given name, argument count and argument types, otherwise
// returns \c false.
static bool methodMatch(const MetaObject *m, int handle,
    const QByteArray &name, int argc,
    const ArgumentType *types)
{
    Q_ASSERT(atoi(priv(m->d.keys)->revision) >= 7);
    if (int(KEY_DATA(m, handle + 1)) != argc)
        return false;

    if (rawStringData(m, KEY_DATA(m, handle)) != name)
        return false;

    int paramsIndex = KEY_DATA(m, handle + 2) + 1;
    for (int i = 0; i < argc; ++i) {
        uint typeInfo = KEY_DATA(m, paramsIndex + i);
        if (types[i].type()) {
            if (types[i].type() != typeFromTypeInfo(m, typeInfo))
                return false;
        }
        else {
            if (types[i].name() != typeNameFromTypeInfo(m, typeInfo))
                return false;
        }
    }

    return true;
}


static void qRemoveWhitespace(const char *s, char *d)
{
    char last = 0;
    while (*s && is_space(*s))
        s++;
    while (*s) {
        while (*s && !is_space(*s))
            last = *d++ = *s++;
        while (*s && is_space(*s))
            s++;
        if (*s && ((is_ident_char(*s) && is_ident_char(last))
            || ((*s == ':') && (last == '<')))) {
            last = *d++ = ' ';
        }
    }
    *d = '\0';
}

static char *qNormalizeType(char *d, int &templdepth, QByteArray &result)
{
    Q_UNUSED(result)
//    const char *t = d;
    while (*d && (templdepth
        || (*d != ',' && *d != ')'))) {
        if (*d == '<')
            ++templdepth;
        if (*d == '>')
            --templdepth;
        ++d;
    }
    // "void" should only be removed if this is part of a signature that has
    // an explicit void argument; e.g., "void foo(void)" --> "void foo()"
    //if (strncmp("void)", t, d - t + 1) != 0)
    //	result += normalizeTypeInternal(t, d);

    return d;
}

// Given a method \a signature (e.g. "foo(int,double)"), this function
// populates the argument \a types array and returns the method name.
QByteArray MetaObjectPrivate::decodeMethodSignature(
    const char *signature, ArgumentTypeArray &types)
{
    Q_ASSERT(signature != 0);
    const char *lparens = strchr(signature, '(');
    if (!lparens)
        return QByteArray();
    const char *rparens = strrchr(lparens + 1, ')');
    if (!rparens || *(rparens + 1))
        return QByteArray();
    int nameLength = lparens - signature;
    argumentTypesFromString(lparens + 1, rparens, types);
    return QByteArray::fromRawData(signature, nameLength);
}

int MetaObjectPrivate::indexOfConstructor(const MetaObject *m, const QByteArray &name, int argc, const ArgumentType *types)
{
    for (int i = atoi(priv(m->d.keys)->constructorCount) - 1; i >= 0; --i) {
        int handle = atoi(priv(m->d.keys)->constructorData) + 5 * i;
        if (methodMatch(m, handle, name, argc, types))
            return i;
    }
    return -1;
}

/*!
    \internal
*/
int MetaObject::static_metacall(Call cl, int idx, void **argv) const
{
    //Q_ASSERT(atoi(priv(d.keys)->revision) >= 6);
    if (!d.static_metacall)
        return 0;
    d.static_metacall(0, cl, idx, argv);
    return -1;
}


/*!
Normalizes the signature of the given \a method.

Qt uses normalized signatures to decide whether two given signals
and slots are compatible. Normalization reduces whitespace to a
minimum, moves 'const' to the front where appropriate, removes
'const' from value types and replaces const references with
values.

\sa checkConnectArgs(), normalizedType()
*/
QByteArray MetaObject::normalizedSignature(const char *method)
{
    QByteArray result;
    if (!method || !*method)
        return result;
    int len = int(strlen(method));
    QVarLengthArray<char> stackbuf(len + 1);
    char *d = stackbuf.data();
    qRemoveWhitespace(method, d);

    result.reserve(len);

    int argdepth = 0;
    int templdepth = 0;
    while (*d) {
        if (argdepth == 1) {
            d = qNormalizeType(d, templdepth, result);
            if (!*d) //most likely an invalid signature.
                break;
        }
        if (*d == '(')
            ++argdepth;
        if (*d == ')')
            --argdepth;
        result += *d++;
    }

    return result;
}


/*!
    \internal
*/
int MetaObject::metacall(TypeObject *object, Call cl, int idx, void **argv)
{
    if (object->d_ptr->metaObject)
        return object->d_ptr->metaObject->metaCall(object, cl, idx, argv);
    else
        return object->cim_metacall(cl, idx, argv);
}

static inline const char *objectClassName(const MetaObject *m)
{
    return rawStringData(m, priv(m->d.keys)->d_className());
}

/*!
    Returns the class name.

    \sa superClass()
*/
const char *MetaObject::className() const
{
    return objectClassName(this);
}

/*!
    \fn MetaObject *MetaObject::superClass() const

    Returns the meta-object of the superclass, or 0 if there is no
    such object.

    \sa className()
*/

/*!
    \internal

    Returns \a obj if object \a obj inherits from this
    meta-object; otherwise returns 0.
*/
TypeObject *MetaObject::cast(TypeObject *obj) const
{
    if (obj) {
        const MetaObject *m = obj->cim_metaObject();
        do {
            if (m == this)
                return obj;
        } while ((m = m->d.superdata));
    }
    return 0;
}

/*!
    \internal

    Returns \a obj if object \a obj inherits from this
    meta-object; otherwise returns 0.
*/
const TypeObject *MetaObject::cast(const TypeObject *obj) const
{
    if (obj) {
        const MetaObject *m = obj->cim_metaObject();
        do {
            if (m == this)
                return obj;
        } while ((m = m->d.superdata));
    }
    return 0;
}

/*!
    Returns the number of properties in this class, including the number of
    properties provided by each base class.

    Use code like the following to obtain a QStringList containing the properties
    specific to a given class:

    \snippet code/src_corelib_kernel_qmetaobject.cpp propertyCount

    \sa property(), propertyOffset(), indexOfProperty()
*/
int MetaObject::propertyCount() const
{
    int n = atoi(priv(d.keys)->propertyCount);
    const MetaObject *m = d.superdata;
    while (m) {
        n += atoi(priv(d.keys)->propertyCount);
        m = m->d.superdata;
    }
    return n;
}

/*!
    Returns the number of items of class information in this class.

    \sa classInfo(), classInfoOffset(), indexOfClassInfo()
*/
int MetaObject::classInfoCount() const
{
    int n = atoi(priv(d.keys)->classInfoCount);
    const MetaObject *m = d.superdata;
    while (m) {
        n += atoi(priv(m->d.keys)->classInfoCount);
        m = m->d.superdata;
    }
    return n;
}


/*!
    Finds property \a name and returns its index; otherwise returns
    -1.

    \sa property(), propertyCount(), propertyOffset()
*/
int MetaObject::indexOfProperty(const char *name) const
{
    const MetaObject *m = this;
    while (m) {
        const MetaObjectPrivate *d = priv(m->d.keys);
        for (int i = atoi(d->propertyCount) - 1; i >= 0; --i)
        {
            int handle = atoi(d->propertyData) + LEN_PROPERTY_FIELD * i;
            const char *varPropName = rawStringData(m, KEY_DATA(m, handle));
            if (name[0] == varPropName[0] && strcmp(name + 1, varPropName + 1) == 0)
            {
                i += m->propertyOffset();
                return i;
            }
        }
        m = m->d.superdata;
    }

    Q_ASSERT(atoi(priv(this->d.keys)->revision) >= 3);

    return -1;
}

/*!
    Finds class information item \a name and returns its index;
    otherwise returns -1.

    \sa classInfo(), classInfoCount(), classInfoOffset()
*/
int MetaObject::indexOfClassInfo(const char *name) const
{
    int i = -1;
    const MetaObject *m = this;
    while (m && i < 0)
    {
        for (i = atoi(priv(m->d.keys)->classInfoCount) - 1; i >= 0; --i)
        {
            int handle = atoi(priv(m->d.keys)->classInfoData) + 2 * i;
            if (strcmp(name, rawStringData(m, KEY_DATA(m, handle))) == 0) {
                i += m->classInfoOffset();
                break;
            }
        }
        m = m->d.superdata;
    }
    return i;
}



/*!
Returns the property offset for this class; i.e. the index
position of this class's first property.

The offset is the sum of all the properties in the class's
superclasses (which is always positive since QObject has the
name() property).

\sa property(), propertyCount(), indexOfProperty()
*/
int MetaObject::propertyOffset() const
{
    int offset = 0;
    const MetaObject *m = d.superdata;
    while (m) {
        offset += atoi(priv(m->d.keys)->propertyCount);
        m = m->d.superdata;
    }
    return offset;
}

/*!
Returns the class information offset for this class; i.e. the
index position of this class's first class information item.

If the class has no superclasses with class information, the
offset is 0; otherwise the offset is the sum of all the class
information items in the class's superclasses.

\sa classInfo(), classInfoCount(), indexOfClassInfo()
*/
int MetaObject::classInfoOffset() const
{
    int offset = 0;
    const MetaObject *m = d.superdata;
    while (m) {
        offset += atoi(priv(m->d.keys)->classInfoCount);
        m = m->d.superdata;
    }
    return offset;
}


/*!
Returns the meta-data for the property with the given \a index.
If no such property exists, a null MetaProperty is returned.

\sa propertyCount(), propertyOffset(), indexOfProperty()
*/
MetaProperty MetaObject::property(int index) const
{
    int i = index;
    i -= propertyOffset();
    if (i < 0 && d.superdata)
        return d.superdata->property(index);

    MetaProperty result;
    if (i >= 0 && i < atoi(priv(d.keys)->propertyCount)) {
        int handle = atoi(priv(d.keys)->propertyData) + LEN_PROPERTY_FIELD * i;
        int flags = KEY_DATA(this, handle + 2);
        result.mobj = this;
        result.handle = handle;
        result.idx = i;

        if (flags & EnumOrFlag) {
//            const char *type = rawTypeNameFromTypeInfo(this, KEY_DATA(this, handle + 1));
            //result.menum = enumerator(indexOfEnumerator(type));
            //if (!result.menum.isValid()) {
            //	const char *enum_name = type;
            //	const char *scope_name = objectClassName(this);
            //	char *scope_buffer = 0;

            //	const char *colon = strrchr(enum_name, ':');
            //	// ':' will always appear in pairs
            //	Q_ASSERT(colon <= enum_name || *(colon - 1) == ':');
            //	if (colon > enum_name) {
            //		int len = colon - enum_name - 1;
            //		scope_buffer = (char *)malloc(len + 1);
            //		memcpy(scope_buffer, enum_name, len);
            //		scope_buffer[len] = '\0';
            //		scope_name = scope_buffer;
            //		enum_name = colon + 1;
            //	}

            //	const QMetaObject *scope = 0;
            //	if (qstrcmp(scope_name, "Qt") == 0)
            //		scope = &QObject::staticQtMetaObject;
            //	else
            //		scope = QMetaObject_findMetaObject(this, scope_name);
            //	if (scope)
            //		result.menum = scope->enumerator(scope->indexOfEnumerator(enum_name));
            //	if (scope_buffer)
            //		free(scope_buffer);
            //}
        }
    }
    return result;
}

/*!
Returns the meta-data for the item of class information with the
given \a index.

Example:

\snippet code/src_corelib_kernel_qmetaobject.cpp 0

\sa classInfoCount(), classInfoOffset(), indexOfClassInfo()
*/
MetaClassInfo MetaObject::classInfo(int index) const
{
    int i = index;
    i -= classInfoOffset();
    if (i < 0 && d.superdata)
        return d.superdata->classInfo(index);

    MetaClassInfo result;
    if (i >= 0 && i < atoi(priv(d.keys)->classInfoCount)) {
        result.mobj = this;
        result.handle = atoi(priv(d.keys)->classInfoData) + 2 * i;
    }
    return result;
}

int MetaObject::objDataCount() const
{
    int nPropCount = propertyCount();
    int nCount = nPropCount;
    for (int i = 0; i < nPropCount; ++i)
    {
        MetaProperty prop = property(i);
        if (prop.isStatic())
            nCount--; // static property is not stored in obj data
    }

    return nCount;
}

/*!
\fn MetaObject::print()
\internal
*/
void MetaObject::print(const char *dirPath) const
{
    if (NULL == dirPath)
        return;

    const QDir printDir(dirPath);
    if (!printDir.exists(printDir.path()))
        printDir.mkdir(printDir.path());

    const QString strFilePath = printDir.path() + QString("/") + className() + ".moc";
    QFile file(strFilePath);
    QTextStream out(&file);

    // create class header file : name.h
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

#define  FIELD_LEN 24
#define FLL(len) qSetFieldWidth(len)

    out.setFieldAlignment(QTextStream::AlignLeft);


    int nTB_LEN = 160;

    // write body
    out << endl << endl
        << FLL(32) << "CIM MOC APPLICATION VERSION :" << FLL(88) << left << "15.0"//IEC61970CIMVersion().Getversion()
        << FLL(0) << "DATE :  " << FLL(20) << QDate::currentDate().toString("yyyy-MM-dd")
        << FLL(0) << endl << endl << endl << endl << endl
        << QByteArray(nTB_LEN * 3 / 4, '*') << endl;

    out	<< FLL(0) << "* Meta Information" << endl
        << FLL(0) << "[ Class ]" << endl
        << FLL(20) << "  Name" << FLL(3) << ":" << FLL(30) << className() << FLL(0) << endl
        << FLL(20) << "  Revision" << FLL(3) << ":" << FLL(30) << atoi(priv(d.keys)->revision) << FLL(0) << endl
        << FLL(20) << "  Inherits" << FLL(3) << ":" << FLL(30) << (d.superdata ? d.superdata->className() : "") << FLL(0) << endl

        << endl;

    if (classInfoCount() > 0)
    {
        out << FLL(0) << "[ Class Info ]" << endl;
        out.setPadChar('-');
        out.setFieldAlignment(QTextStream::AlignCenter);
        out << FLL(0) << "/" << FLL(nTB_LEN / 4) << "Key"
            << FLL(0) << "//" << FLL(nTB_LEN * 3 / 4) << "Value" << FLL(0) << "/" << endl;

        out.setPadChar(' ');
        for (int i = 0; i < classInfoCount(); ++i)
            out << FLL(0) << "  " << FLL(nTB_LEN / 4) << left << classInfo(i).name()
            << FLL(nTB_LEN * 3 / 4) << left << classInfo(i).value() << FLL(0) << endl;

        out << endl;
    }

    if (propertyCount() > 0)
    {
        out << FLL(0) << "[ Properties ]" << endl;
        out.setPadChar('-');
        out.setFieldAlignment(QTextStream::AlignCenter);
        out << FLL(0) << "/" << FLL(nTB_LEN * 2/ 5) << "Name"
            << FLL(0) << "//" << FLL(nTB_LEN * 1 / 5) << "Type"
            << FLL(0) << "//" << FLL(nTB_LEN * 3 / 10) << "Flags"
            << FLL(0) << "//" << FLL(nTB_LEN * 1 / 10) << "From" << FLL(0) << "/" << endl;

        for (int i = 0; i < propertyCount(); ++i)
        {
            out.setPadChar(' ');
            MetaProperty prop = property(i);
            out << FLL(0) << "  "
                << FLL(nTB_LEN * 2 / 5) << left << property(i).name()
                << FLL(nTB_LEN * 1 / 5) << center << property(i).typeName()
                << FLL(nTB_LEN * 3 / 10) << right << "Readable/Editable/Visible"
                << FLL(nTB_LEN * 1 / 10) << right << "Local"
                << FLL(0) << endl;

            if (prop.propertyInfoCount() > 0)
            {
                out << FLL(nTB_LEN * 1 / 10) << " ";
                out.setPadChar('.');
                out << FLL(0) << "/" << FLL(nTB_LEN * 3 / 10) << center << "Info Name"
                    << FLL(0) << "//" << FLL(nTB_LEN * 6 / 10) << center << "Info Value"
                    << FLL(0) << "/" << endl;

                out.setPadChar(' ');
                for (int j = 0; j < prop.propertyInfoCount(); ++j)
                {
                    out << FLL(nTB_LEN * 1 / 10) << " "
                        << FLL(nTB_LEN * 3 / 10) << left << prop.propertyInfo(j).name()
                        << FLL(nTB_LEN * 6 / 10) << left << prop.propertyInfo(j).value()
                        << FLL(0) << endl;
                }
            }
        }
        out << endl;

    }

    // out Cipher Text
    out << FLL(0) << endl << endl << endl;

    out << FLL(0) << "* Cipher Information" << endl
        << FLL(0) << "[ QString Data ]" << endl;

    out << d.stringdata << endl << endl;
    // pretty text:
    out << FLL(0) << "[ Pretty QString Data ]" << endl;
    int nCount = d.stringdata.count('\0');
    for (int i = 0; i < nCount; ++i)
    {
        if (i != 0)
            out << FLL(0) << '\n';
        out << FLL(0) << "\t[ " << i << " ] :" << constData_Bi(d.stringdata, i);
    }
    out << endl << endl;

    out << FLL(0) << "[ Key Data ]" << endl;
    out << d.keys << endl << endl;
    // pretty text:
    out << FLL(0) << "[ Pretty Key Data ]" << endl;
    int nNextPos = 0;
    nCount = d.keys.count('\0');
#define KEY_STRING(n, w) FLL(w) << constData_Bi(d.keys, n) << FLL(0)
    out << FLL(0) << " // content:" << endl
        << FLL(0) << "\t" << KEY_STRING(0, 0) << FLL(30) << "," << FLL(0) << "// revision" << endl
        << FLL(0) << "\t" << KEY_STRING(1, 0) << FLL(30) << "," << FLL(0) << "// classname" << endl
        << FLL(0) << "\t" << KEY_STRING(2, 0) << FLL(5) << ", " << KEY_STRING(3, 0) << FLL(30) << "," << FLL(0) << "// classinfo" << endl
        << FLL(0) << "\t" << KEY_STRING(4, 0) << FLL(5) << ", " << KEY_STRING(5, 0) << FLL(30) << "," << FLL(0) << "// methods" << endl
        << FLL(0) << "\t" << KEY_STRING(6, 0) << FLL(5) << ", " << KEY_STRING(7, 0) << FLL(30) << "," << FLL(0) << "// properties/infos" << endl
        << FLL(0) << "\t" << KEY_STRING(8, 0) << FLL(5) << ", " << KEY_STRING(9, 0) << FLL(30) << "," << FLL(0) << "// enums/sets" << endl
        << FLL(0) << "\t" << KEY_STRING(10, 0) << FLL(5) << ", " << KEY_STRING(11, 0) << FLL(30) << "," << FLL(0) << "// constructors" << endl
        << FLL(0) << "\t" << KEY_STRING(12, 0) << FLL(30) << "," << FLL(0) << "// flags" << endl
        << FLL(0) << "\t" << KEY_STRING(13, 0) << FLL(30) << "," << FLL(0) << "// signalCount" << endl << endl;

    out << FLL(0) << " // classinfo: key, value" << endl;
    nNextPos = 14;
    for (int i = 0; i < classInfoCount(); ++i)
    {
        out << FLL(0) << "\t" << KEY_STRING(nNextPos + 2 * i, 0)
            << FLL(10) << ",   " << KEY_STRING(nNextPos + 2 * i + 1, 0)
            << FLL(10) << "," << FLL(0) << endl;
    }
    nNextPos += 2 * classInfoCount();
    out << endl;

    out << FLL(0) << " // properties: name, type, flags, info.count, info.pos, value.startpos, value.endpos" << endl;
    for (int i = 0; i < propertyCount(); ++i)
    {
        out << FLL(0) << "\t";
        for (int j = 0; j < LEN_PROPERTY_FIELD; ++j)
        {
            if (j != 0)
                out << FLL(10) << ", ";
            out << KEY_STRING(nNextPos + LEN_PROPERTY_FIELD * i + j, 0);
        }
        out << FLL(10) << ",   " << FLL(0) << endl;
    }
    nNextPos += LEN_PROPERTY_FIELD * propertyCount();
    out << endl;

    out << FLL(0) << " // properties' infos: key value" << endl;
    for (int i = 0; i < propertyCount(); ++i)
    {
        MetaProperty varProp = property(i);
        for (int j = 0; j < varProp.propertyInfoCount(); ++j)
        {
            out << FLL(0) << "\t" << KEY_STRING(nNextPos + 2 * j, 0)
                << FLL(10) << ", " << KEY_STRING(nNextPos + 2 * j + 1, 0)
                << FLL(10) << "," << FLL(0) << endl;
        }
        nNextPos += 2 * varProp.propertyInfoCount();
        out << "\t//------------" << endl;
    }

    out << FLL(0) << endl << endl << endl << endl << endl
        << FLL(0) << QByteArray(5, '*')
        << FLL(7) << center << "END"
        << FLL(0) << QByteArray(5, '*');

    out.flush();
    file.close();

    return;
}


/*!
\since 4.5

Finds \a constructor and returns its index; otherwise returns -1.

Note that the \a constructor has to be in normalized form, as returned
by normalizedSignature().

\sa constructor(), constructorCount(), normalizedSignature()
*/
int MetaObject::indexOfConstructor(const char *constructor) const
{
    Q_ASSERT(atoi(priv(d.keys)->revision) >= 7);
    ArgumentTypeArray types;
    QByteArray name = MetaObjectPrivate::decodeMethodSignature(constructor, types);
    return MetaObjectPrivate::indexOfConstructor(this, name, types.size(), types.constData());
}


/*!
    \fn MetaEnum::MetaEnum()
    \internal
*/

/*!
    Returns the name of the enumerator (without the scope).

    For example, the Qt::AlignmentFlag enumeration has \c
    AlignmentFlag as the name and \l Qt as the scope.

    \sa isValid(), scope()
*/
const char *MetaEnum::name() const
{
    if (!mobj)
        return 0;
    return rawStringData(mobj, KEY_DATA(mobj, handle));
}


/*!
    \since 4.5

    Constructs a new instance of this class. You can pass up to ten arguments
    (\a val0, \a val1, \a val2, \a val3, \a val4, \a val5, \a val6, \a val7,
    \a val8, and \a val9) to the constructor. Returns the new object, or 0 if
    no suitable constructor is available.

    Note that only constructors that are declared with the Q_INVOKABLE
    modifier are made available through the meta-object system.

    \sa Q_ARG(), constructor()
*/

QVariant MetaObject::createTypeInstance(const char *name,
                                      QByteArray val0,
                                      QByteArray val1,
                                      QByteArray val2,
                                      QByteArray val3,
                                      QByteArray val4,
                                      QByteArray val5,
                                      QByteArray val6,
                                      QByteArray val7,
                                      QByteArray val8,
                                      QByteArray val9)
{
    if (NULL == name || strcmp(name, "") == 0)
        return QVariant();

    if (NULL == qMapMetaObject(name))
        return QVariant();

    InstanceType type = StackInstance;
    if (name[qstrlen(name) - 1] == '*')
        type = HeapInstance;

    MetaObject *mobj = qMapMetaObject(name);
    Q_ASSERT(Q_NULLPTR != mobj);
    return mobj->createInstance(type, val0, val1, val2, val3, val4, val5,
        val6, val7, val8, val9);
}

QVariant MetaObject::createInstance(InstanceType type,
                                    QByteArray val0,
                                    QByteArray val1,
                                    QByteArray val2,
                                    QByteArray val3,
                                    QByteArray val4,
                                    QByteArray val5,
                                    QByteArray val6,
                                    QByteArray val7,
                                    QByteArray val8,
                                    QByteArray val9) const
{
    QByteArray constructorName = className();
    {
        int idx = constructorName.lastIndexOf(':');
        if (idx != -1)
            constructorName.remove(0, idx + 1); // remove qualified part
    }

    QByteArray sig = constructorName;
    sig.append("()");
    sig.append('\0');

    QVariant returnValue;
    void *param[] = { &returnValue, &type, &val0, &val1, &val2, &val3,
        &val4, &val5, &val6, &val7, &val8, &val9 };

    if (static_metacall(CreateInstance, 0, param) >= 0)
        return 0;
    return returnValue;
}

QVariant MetaObject::retrieveObjData(QVariant &varObj) const
{
    QByteArray constructorName = className();
    {
        int idx = constructorName.lastIndexOf(':');
        if (idx != -1)
            constructorName.remove(0, idx + 1); // remove qualified part
    }

    QByteArray sig = constructorName;
    sig.append("()");
    sig.append('\0');

    QVariant returnValue;
    void *param[] = { &returnValue, &varObj};

    if (static_metacall(RetrieveObjData, 0, param) >= 0)
        return 0;
    return returnValue;
}

/*!
    Returns the number of keys.

    \sa key()
*/
int MetaEnum::keyCount() const
{
    if (!mobj)
        return 0;
    return KEY_DATA(mobj, handle + 2);
}


/*!
    Returns the key with the given \a index, or 0 if no such key exists.

    \sa keyCount(), value(), valueToKey()
*/
const char *MetaEnum::key(int index) const
{
    if (!mobj)
        return 0;
    int count = KEY_DATA(mobj, handle + 2);
    int data = KEY_DATA(mobj, handle + 3);
    if (index >= 0  && index < count)
        return rawStringData(mobj, KEY_DATA(mobj, data + 2*index));
    return 0;
}

/*!
    Returns the value with the given \a index; or returns -1 if there
    is no such value.

    \sa keyCount(), key(), keyToValue()
*/
int MetaEnum::value(int index) const
{
    if (!mobj)
        return 0;
    int count = KEY_DATA(mobj, handle + 2);
    int data = KEY_DATA(mobj, handle + 3);
    if (index >= 0  && index < count)
        return KEY_DATA(mobj, data + 2 * index + 1);
    return -1;
}


/*!
    Returns \c true if this enumerator is used as a flag; otherwise returns
    false.

    When used as flags, enumerators can be combined using the OR
    operator.

    \sa keysToValue(), valueToKeys()
*/
bool MetaEnum::isFlag() const
{
    return mobj && KEY_DATA(mobj, handle + 1);
}


/*!
    Returns the scope this enumerator was declared in.

    For example, the Qt::AlignmentFlag enumeration has \c Qt as
    the scope and \c AlignmentFlag as the name.

    \sa name()
*/
const char *MetaEnum::scope() const
{
    return mobj ? objectClassName(mobj) : 0;
}

/*!
    Returns the integer value of the given enumeration \a key, or -1
    if \a key is not defined.

    If \a key is not defined, *\a{ok} is set to false; otherwise
    *\a{ok} is set to true.

    For flag types, use keysToValue().

    \sa valueToKey(), isFlag(), keysToValue()
*/
int MetaEnum::keyToValue(const char *key, bool *ok) const
{
    if (ok != 0)
        *ok = false;
    if (!mobj || !key)
        return -1;
    uint scope = 0;
    const char *qualified_key = key;
    const char *s = key + qstrlen(key);
    while (s > key && *s != ':')
        --s;
    if (s > key && *(s-1)==':') {
        scope = s - key - 1;
        key += scope + 2;
    }
    int count = KEY_DATA(mobj, handle + 2);
    int data = KEY_DATA(mobj, handle + 3);
    for (int i = 0; i < count; ++i)
    {

        const QByteArray className = constData_Bi(mobj->d.stringdata, atoi(priv(mobj->d.keys)->className));
        if ((!scope || (className.size() == int(scope) && strncmp(qualified_key, className.constData(), scope) == 0))
             && strcmp(key, rawStringData(mobj, KEY_DATA(mobj, data + 2*i))) == 0) {
            if (ok != 0)
                *ok = true;
            return KEY_DATA(mobj, data + 2*i + 1);
        }
    }
    return -1;
}

/*!
    Returns the string that is used as the name of the given
    enumeration \a value, or 0 if \a value is not defined.

    For flag types, use valueToKeys().

    \sa isFlag(), valueToKeys()
*/
const char* MetaEnum::valueToKey(int value) const
{
    if (!mobj)
        return 0;
    int count = KEY_DATA(mobj, handle + 2);
    int data = KEY_DATA(mobj, handle + 3);
    for (int i = 0; i < count; ++i)
        if (value == (int)KEY_DATA(mobj, data + 2*i + 1))
            return rawStringData(mobj, KEY_DATA(mobj, data + 2*i));
    return 0;
}

/*!
    Returns the value derived from combining together the values of
    the \a keys using the OR operator, or -1 if \a keys is not
    defined. Note that the strings in \a keys must be '|'-separated.

    If \a keys is not defined, *\a{ok} is set to false; otherwise
    *\a{ok} is set to true.

    \sa isFlag(), valueToKey(), valueToKeys()
*/
int MetaEnum::keysToValue(const char *keys, bool *ok) const
{
    if (ok != 0)
        *ok = false;
    if (!mobj || !keys)
        return -1;
    if (ok != 0)
        *ok = true;
    QStringList l = QString::fromLatin1(keys).split(QLatin1Char('|'));
    if (l.isEmpty())
        return 0;
    //#### TODO write proper code, do not use QStringList
    int value = 0;
    int count = KEY_DATA(mobj, handle + 2);
    int data = KEY_DATA(mobj, handle + 3);
    for (int li = 0; li < l.size(); ++li) {
        QString trimmed = l.at(li).trimmed();
        QByteArray qualified_key = trimmed.toLatin1();
        const char *key = qualified_key.constData();
        uint scope = 0;
        const char *s = key + qstrlen(key);
        while (s > key && *s != ':')
            --s;
        if (s > key && *(s-1)==':') {
            scope = s - key - 1;
            key += scope + 2;
        }
        int i;
        for (i = count-1; i >= 0; --i)
        {

            const QByteArray className = constData_Bi(mobj->d.stringdata, atoi(priv(mobj->d.keys)->className));
            if ((!scope || (className.size() == int(scope) && strncmp(qualified_key.constData(), className.constData(), scope) == 0))
                 && strcmp(key, rawStringData(mobj, KEY_DATA(mobj, data + 2*i))) == 0) {
                value |= KEY_DATA(mobj, data + 2*i + 1);
                break;
            }
        }
        if (i < 0) {
            if (ok != 0)
                *ok = false;
            value |= -1;
        }
    }
    return value;
}

/*!
    Returns a byte array of '|'-separated keys that represents the
    given \a value.

    \sa isFlag(), valueToKey(), keysToValue()
*/
QByteArray MetaEnum::valueToKeys(int value) const
{
    QByteArray keys;
    if (!mobj)
        return keys;
    int count = KEY_DATA(mobj, handle + 2);
    int data = KEY_DATA(mobj, handle + 3);
    int v = value;
    // reverse iterate to ensure values like Qt::Dialog=0x2|Qt::Window are processed first.
    for (int i = count - 1; i >= 0; --i) {
        int k = KEY_DATA(mobj, data + 2*i + 1);
        if ((k != 0 && (v & k) == k ) ||  (k == value))  {
            v = v & ~k;
            if (!keys.isEmpty())
                keys.prepend('|');
            keys.prepend(rawStringData(mobj, KEY_DATA(mobj, data + 2*i)));
        }
    }
    return keys;
}

/*!
    \fn MetaEnum MetaEnum::fromType()
    \since 5.5

    Returns the MetaEnum corresponding to the type in the template parameter.
    The enum needs to be declared with Q_ENUM.
*/

static QByteArray qualifiedName(const MetaEnum &e)
{
    return QByteArray(e.scope()) + "::" + e.name();
}

/*!
    \class MetaProperty
    \inmodule QtCore
    \brief The MetaProperty class provides meta-data about a property.

    \ingroup objectmodel

    Property meta-data is obtained from an object's meta-object. See
    MetaObject::property() and MetaObject::propertyCount() for
    details.

    \section1 Property Meta-Data

    A property has a name() and a type(), as well as various
    attributes that specify its behavior: isReadable(), isWritable(),
    isDesignable(), isScriptable(), revision(), and isStored().

    If the property is an enumeration, isEnumType() returns \c true; if the
    property is an enumeration that is also a flag (i.e. its values
    can be combined using the OR operator), isEnumType() and
    isFlagType() both return true. The enumerator for these types is
    available from enumerator().

    The property's values are set and retrieved with read(), write(),
    and reset(); they can also be changed through TypeObject's set and get
    functions. See TypeObject::setProperty() and TypeObject::property() for
    details.

    \section1 Copying and Assignment

    MetaProperty objects can be copied by value. However, each copy will
    refer to the same underlying property meta-data.

    \sa MetaObject, MetaEnum, MetaMethod, {Qt's Property System}
*/

/*!
    \fn bool MetaProperty::isValid() const

    Returns \c true if this property is valid (readable); otherwise
    returns \c false.

    \sa isReadable()
*/

/*!
    \fn const MetaObject *MetaProperty::enclosingMetaObject() const
    \internal
*/

/*!
    \internal
*/
MetaProperty::MetaProperty(QObject *parent)
    : mobj(0), handle(0), idx(0)
{
    Q_UNUSED(parent);
}


/*!
    Returns this property's name.

    \sa type(), typeName()
*/
const char *MetaProperty::name() const
{
    if (!mobj)
        return 0;
    int handle = atoi(priv(mobj->d.keys)->propertyData) + LEN_PROPERTY_FIELD*idx;
    return rawStringData(mobj, KEY_DATA(mobj, handle));
}

/*!
    Returns the name of this property's type.

    \sa type(), name()
*/
const char *MetaProperty::typeName() const
{
    if (!mobj)
        return 0;
//    int id0 = KEY_DATA(mobj, handle + 1);
    return rawStringData(mobj, KEY_DATA(mobj, handle + 1));
}

/*!
    Returns this property's type. The return value is one
    of the values of the QVariant::Type enumeration.

    \sa userType(), typeName(), name()
*/
QVariant::Type MetaProperty::type() const
{
    if (!mobj)
        return QVariant::Invalid;

    Q_ASSERT(atoi(priv(mobj->d.keys)->revision) >= 7);
    uint type = QMetaType::type(rawStringData(mobj, KEY_DATA(mobj, handle + 1)));
    if (type >= QMetaType::User)
        return QVariant::UserType;
    if (type != QMetaType::UnknownType)
        return QVariant::Type(type);
    if (isEnumType()) {
        int enumMetaTypeId = QMetaType::type(qualifiedName(menum));
        if (enumMetaTypeId == QMetaType::UnknownType)
            return QVariant::Int;
    }

    return QVariant::UserType;
}

/*!
    \since 4.2

    Returns this property's user type. The return value is one
    of the values that are registered with QMetaType, or QMetaType::UnknownType if
    the type is not registered.

    \sa type(), QMetaType, typeName()
 */
int MetaProperty::userType() const
{
    if (!mobj)
        return QMetaType::UnknownType;
    Q_ASSERT(atoi(priv(mobj->d.keys)->revision) >= 7);
    uint type = QMetaType::type(rawStringData(mobj, KEY_DATA(mobj, handle + 1)));
    if (type != QMetaType::UnknownType)
        return type;
    if (isEnumType()) {
        type = QMetaType::type(qualifiedName(menum));
        if (type == QMetaType::UnknownType) {
            type = registerPropertyType();
            if (type == QMetaType::UnknownType)
                return QVariant::Int; // Match behavior of QMetaType::type()
        }
        return type;
    }
    type = QMetaType::type(typeName());
    if (type != QMetaType::UnknownType)
        return type;
    return registerPropertyType();
}

/*!
  \since 4.6

  Returns this property's index.
*/
int MetaProperty::propertyIndex() const
{
    if (!mobj)
        return -1;
    return idx + mobj->propertyOffset();
}

int MetaProperty::propertyFlags() const
{
    if (!mobj)
        return -1;

    return KEY_DATA(mobj, handle + 2);
}

QVariant MetaProperty::value() const
{ // field idx = 6
    if (!mobj)
        return -1;

    QVariant v;

    int idx_start = KEY_DATA(mobj, handle + 5);
    int idx_end = KEY_DATA(mobj, handle + 6);

    if (idx_start < 0 || idx_end < 0 || idx_end < idx_start)
        return QVariant();

    QByteArray param = rawObjData(mobj, idx_start, idx_end);
    if (userType() >= QMetaType::User)
    {
        if (!isPointer())
            v.setValue(MetaObject::createTypeInstance(typeName(), param));
        else
        {
            MetaObject &mo = *qMapMetaObject(typeName());
            v.setValue(mo.createInstance(PointerInstance, param));
        }
    }
    else
    {
        v.setValue(param);
    }

    return v;
}

bool MetaProperty::setValue(const QVariant &newVar)
{ // field idx = 6
    if (!mobj || !isValid())
        return false;

    QVariant var = newVar;
    if (value() == var)
        return false;

    // update value
    if (strcmp(typeName(), var.typeName()) != 0)
    {
        setData_Bij((const_cast<MetaObject*>(mobj))->d.stringdata
            , KEY_DATA(mobj, handle + 1), KEY_DATA(mobj, handle + 1)
            , QByteArray(var.typeName()));
        Q_ASSERT(strcmp(typeName(), QMetaType::typeName(userType())) == 0);

        if (var.typeName()[qstrlen(var.typeName()) - 1] == '*')
            setFlag(Pointer);
    }

    QByteArray bytes;
    if (QMetaType::type(var.typeName()) >= QMetaType::User)
    {
        if (!isPointer())
        {
            MetaObject *var_mobj = qMapMetaObject(var.typeName());
            Q_ASSERT(NULL != var_mobj);
            //if (NULL == var_mobj)
            //qDebug() << var.typeName();
            bytes = var_mobj->retrieveObjData(var).toByteArray();
            //qDebug() << bytes;
        }
        else
        {
            bytes.clear();
            var.setValue(var.data_ptr().data.ptr);
            bytes ooon(qulonglong(var.data_ptr().data.ptr));
        }
    }
    else if (var.canConvert<QByteArray>())
        bytes = newVar.toByteArray();
    if (bytes.lastIndexOf('\0') != bytes.length() - 1)
        bytes.append('\0');

    int idx0 = KEY_DATA(mobj, handle + 5);
    int idx1 = KEY_DATA(mobj, handle + 6);
    setData_Bij((const_cast<MetaObject*>(mobj))->d.objdata
        , idx0, idx1, bytes);

    int nAdd = bytes.count('\0') - 1 - (idx1 - idx0);
    if (idx1 - idx0 != nAdd)
        setData_Bij((const_cast<MetaObject*>(mobj))->d.keys,
        handle + 6, handle + 6, QByteArray::number(idx1 + nAdd));

    for (int i = idx + 1; i < mobj->propertyCount(); ++i)
    {
        int new_handle = handle + LEN_PROPERTY_FIELD * (i - idx);
        idx0 = KEY_DATA(mobj, new_handle + 5);
        idx1 = KEY_DATA(mobj, new_handle + 6);

        setData_Bij((const_cast<MetaObject*>(mobj))->d.keys,
            new_handle + 5, new_handle + 5, QByteArray::number(idx0 + nAdd));
        setData_Bij((const_cast<MetaObject*>(mobj))->d.keys,
            new_handle + 6, new_handle + 6, QByteArray::number(idx1 + nAdd));
    }

    return true;
}

/*!
\since 4.6

Returns this property's info count.
*/
int MetaProperty::propertyInfoCount() const
{
    if (!mobj)
        return 0;
    return KEY_DATA(mobj, handle + 3);
}


/*!
Finds class information item \a name and returns its index;
otherwise returns -1.

\sa propertyInfo(), propertyInfoCount()
*/
int MetaProperty::indexOfPropertyInfo(const char *name) const
{
    int i = -1;
    const MetaObject *m = mobj;
    while (m && i < 0)
    {
        for (i = KEY_DATA(m, handle + 3) - 1; i >= 0; --i)
        {
            int infoHandle = KEY_DATA(m, handle + 4) + 2 * i;
//            const char *varInfoName = rawStringData(m, KEY_DATA(m, infoHandle));
            if (strcmp(name, rawStringData(m, KEY_DATA(m, infoHandle))) == 0)
            {
                i += m->classInfoOffset();
                break;
            }
        }
        m = m->d.superdata;
    }
    return i;
}

/*!
    Returns the meta-data for the item of class information with the
    given \a index.

*/
MetaPropertyInfo MetaProperty::propertyInfo(int index) const
{
    int i = index;

    uint propertyInfoData = KEY_DATA(mobj, handle + 4);
    MetaPropertyInfo result;
    if (i >= 0 && i < propertyInfoCount()) {
        result.mobj = this->mobj;
        result.handle = propertyInfoData + 2 * i;	// 2 is propertyInfo's fields count
        result.idx_prop = idx;
        result.idx = i;
    }

    return result;
}

/*!
    Returns \c true if the property's type is an enumeration value that
    is used as a flag; otherwise returns \c false.

    Flags can be combined using the OR operator. A flag type is
    implicitly also an enum type.

    \sa isEnumType(), enumerator(), MetaEnum::isFlag()
*/

bool MetaProperty::isFlagType() const
{
    return isEnumType() && menum.isFlag();
}

/*!
    Returns \c true if the property's type is an enumeration value;
    otherwise returns \c false.

    \sa enumerator(), isFlagType()
*/
bool MetaProperty::isEnumType() const
{
    if (!mobj)
        return false;
    int handle = atoi(priv(mobj->d.keys)->propertyData) + LEN_PROPERTY_FIELD * idx;
    int flags = KEY_DATA(mobj, handle + 2);
    return (flags & EnumOrFlag) && menum.name();
}

/*!
    \internal
    Executes metacall with MetaObject::RegisterPropertyMetaType flag.
    Returns id of registered type or QMetaType::UnknownType if a type
    could not be registered for any reason.
*/
int MetaProperty::registerPropertyType() const
{
    int registerResult = -1;
    void *argv[] = { &registerResult };
    mobj->static_metacall(MetaObject::RegisterPropertyMetaType, idx, argv);
    return registerResult == -1 ? QMetaType::UnknownType : registerResult;
}

/*!
    Returns the enumerator if this property's type is an enumerator
    type; otherwise the returned value is undefined.

    \sa isEnumType(), isFlagType()
*/
MetaEnum MetaProperty::enumerator() const
{
    return menum;
}

/*!
    Reads the property's value from the given \a object. Returns the value
    if it was able to read it; otherwise returns an invalid variant.

    \sa write(), reset(), isReadable()
*/
QVariant MetaProperty::read(const TypeObject *object) const
{
    if (!object || !mobj)
        return QVariant();

    uint t = QVariant::Int;
    if (isEnumType()) {
        /*
          try to create a QVariant that can be converted to this enum
          type (only works if the enum has already been registered
          with QMetaType)
        */
    } else {

    }

    // the status variable is changed by cim_metacall to indicate what it did
    // this feature is currently only used by Qt D-Bus and should not be depended
    // upon. Don't change it without looking into QDBusAbstractInterface first
    // -1 (unchanged): normal cim_metacall, result stored in argv[0]
    // changed: result stored directly in value
    int status = -1;
    QVariant value;
    void *argv[] = { 0, &value, &status };
    if (t == QMetaType::QVariant) {
        argv[0] = &value;
    } else {
        value = QVariant(t, (void*)0);
        argv[0] = value.data();
    }

    if (status != -1)
        return value;

    return value;
}

bool MetaProperty::setFlag(int flag, bool enabled)
{
    if (!isWritable())
        return false;

    int flags = KEY_DATA(mobj, handle + 2);
    if (enabled)
        setFlags(PropertyFlags(flags) | flag);
    else
        setFlags(PropertyFlags(flags) & ~flag);

    return true;
}

bool MetaProperty::setFlags(int flags)
{
    if (!isWritable() || flags < 0)
        return false;

    int old_flags = KEY_DATA(mobj, handle + 2);
    if (quint32(old_flags) == quint32(flags))
        return false;

    // update flags
    setData_Bij((const_cast<MetaObject*>(mobj))->d.keys
        , handle + 2, handle + 2, QByteArray::number(flags));

    return true;
}

/*!
    Writes \a value as the property's value to the given \a object. Returns
    true if the write succeeded; otherwise returns \c false.

    \sa read(), reset(), isWritable()
*/
bool MetaProperty::write(TypeObject *object, const QVariant &value) const
{
    if (!object || !isWritable())
        return false;

    QVariant v = value;
    uint t = QVariant::Invalid;
    if (isEnumType()) {
        if (v.type() == QVariant::String) {
            bool ok;
            if (isFlagType())
                v = QVariant(menum.keysToValue(value.toByteArray(), &ok));
            else
                v = QVariant(menum.keyToValue(value.toByteArray(), &ok));
            if (!ok)
                return false;
        } else if (v.type() != QVariant::Int && v.type() != QVariant::UInt) {
            int enumMetaTypeId = QMetaType::type(qualifiedName(menum));
            if ((enumMetaTypeId == QMetaType::UnknownType) || (v.userType() != enumMetaTypeId) || !v.constData())
                return false;
            v = QVariant(*reinterpret_cast<const int *>(v.constData()));
        }
        v.convert(QVariant::Int);
    } else {
        int handle = atoi(priv(mobj->d.keys)->propertyData) + LEN_PROPERTY_FIELD * idx;
        const char *typeName = 0;
        Q_ASSERT(atoi(priv(mobj->d.keys)->revision) >= 7);
        uint typeInfo = KEY_DATA(mobj, handle + 1);
        if (!(typeInfo & IsUnresolvedType))
            t = typeInfo;
        else {
            typeName = rawStringData(mobj, typeInfo & TypeNameIndexMask);
            t = QMetaType::type(typeName);
            if (t == QMetaType::UnknownType)
                t = registerPropertyType();
            if (t == QMetaType::UnknownType)
                return false;
        }
        if (t != QMetaType::QVariant && t != (uint)value.userType() && (t < QMetaType::User && !v.convert((QVariant::Type)t)))
            return false;
    }

    // the status variable is changed by cim_metacall to indicate what it did
    // this feature is currently only used by Qt D-Bus and should not be depended
    // upon. Don't change it without looking into QDBusAbstractInterface first
    // -1 (unchanged): normal cim_metacall, result stored in argv[0]
    // changed: result stored directly in value, return the value of status
    int status = -1;
    // the flags variable is used by the declarative module to implement
    // interception of property writes.
    int flags = 0;
    void *argv[] = { 0, &v, &status, &flags };
    if (t == QMetaType::QVariant)
        argv[0] = &v;
    else
        argv[0] = v.data();
    if (atoi(priv(mobj->d.keys)->flags) & PropertyAccessInStaticMetaCall && mobj->d.static_metacall)
        mobj->d.static_metacall(object, MetaObject::WriteProperty, idx, argv);

    return status;
}

/*!
    Resets the property for the given \a object with a reset method.
    Returns \c true if the reset worked; otherwise returns \c false.

    Reset methods are optional; only a few properties support them.

    \sa read(), write()
*/
bool MetaProperty::reset(TypeObject *object) const
{
    if (!object || !mobj || !isResettable())
        return false;
    void *argv[] = { 0 };
    if (atoi(priv(mobj->d.keys)->flags) & PropertyAccessInStaticMetaCall && mobj->d.static_metacall)
        mobj->d.static_metacall(object, MetaObject::ResetProperty, idx, argv);

    return true;
}
/*!
    \since 5.5

    Reads the property's value from the given \a gadget. Returns the value
    if it was able to read it; otherwise returns an invalid variant.

    This function should only be used if this is a property of a Q_GADGET
*/
QVariant MetaProperty::readOnGadget(const void *gadget) const
{
    Q_ASSERT(atoi(priv(mobj->d.keys)->flags) & PropertyAccessInStaticMetaCall && mobj->d.static_metacall);
    return read(reinterpret_cast<const TypeObject*>(gadget));
}

/*!
    \since 5.5

    Writes \a value as the property's value to the given \a gadget. Returns
    true if the write succeeded; otherwise returns \c false.

    This function should only be used if this is a property of a Q_GADGET
*/
bool MetaProperty::writeOnGadget(void *gadget, const QVariant &value) const
{
    Q_ASSERT(atoi(priv(mobj->d.keys)->flags) & PropertyAccessInStaticMetaCall && mobj->d.static_metacall);
    return write(reinterpret_cast<TypeObject*>(gadget), value);
}

/*!
    \since 5.5

    Resets the property for the given \a gadget with a reset method.
    Returns \c true if the reset worked; otherwise returns \c false.

    Reset methods are optional; only a few properties support them.

    This function should only be used if this is a property of a Q_GADGET
*/
bool MetaProperty::resetOnGadget(void *gadget) const
{
    Q_ASSERT(atoi(priv(mobj->d.keys)->flags) & PropertyAccessInStaticMetaCall && mobj->d.static_metacall);
    return reset(reinterpret_cast<TypeObject*>(gadget));
}

/*!
    Returns \c true if this property can be reset to a default value; otherwise
    returns \c false.

    \sa reset()
*/
bool MetaProperty::isResettable() const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    return flags & Resettable;
}

/*!
    Returns \c true if this property is readable; otherwise returns \c false.

    \sa isWritable(), read(), isValid()
 */
bool MetaProperty::isReadable() const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    return flags & Readable;
}

// This method has been around for a while, but the documentation was marked \internal until 5.1
/*!
    \since 5.1

    Returns the property revision if one was
    specified by REVISION, otherwise returns 0.
 */
int MetaProperty::revision() const
{
    if (!mobj)
        return 0;

    int flags = KEY_DATA(mobj, handle + 2);
    if (flags & Revisioned) {
        int offset = atoi(priv(mobj->d.keys)->propertyData) +
                     atoi(priv(mobj->d.keys)->propertyCount) * 3 + idx;
        // Revision data is placed after NOTIFY data, if present.
        // Iterate through properties to discover whether we have NOTIFY signals.
        for (int i = 0; i < atoi(priv(mobj->d.keys)->propertyCount); ++i) {
            int handle = atoi(priv(mobj->d.keys)->propertyData) + 3*i;
            if (KEY_DATA(mobj, handle + 2) & Notify) {
                offset += atoi(priv(mobj->d.keys)->propertyCount);
                break;
            }
        }
        return KEY_DATA(mobj, offset);
    } else {
        return 0;
    }
}

/*!
    Returns \c true if this property is writable; otherwise returns
    false.

    \sa isReadable(), write()
 */
bool MetaProperty::isWritable() const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    return flags & Writable;
}

/*!
    Returns \c true if this property is visible to user; otherwise returns
    false.

\sa isReadable(), write()
*/
bool MetaProperty::isVisiable() const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    return flags & Visible;
}


/*!
    Returns \c true if this property is designable for the given \a object;
    otherwise returns \c false.

    If no \a object is given, the function returns \c false if the
    \c{Q_PROPERTY()}'s \c DESIGNABLE attribute is false; otherwise
    returns \c true (if the attribute is true or is a function or expression).

    \sa isScriptable(), isStored()
*/
bool MetaProperty::isDesignable(const TypeObject *object) const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    bool b = flags & Designable;
    if (object) {
        void *argv[] = { &b };
        MetaObject::metacall(const_cast<TypeObject*>(object), MetaObject::QueryPropertyDesignable,
                              idx /*+ mobj->propertyOffset()*/, argv);
    }
    return b;


}

/*!
    Returns \c true if the property is scriptable for the given \a object;
    otherwise returns \c false.

    If no \a object is given, the function returns \c false if the
    \c{Q_PROPERTY()}'s \c SCRIPTABLE attribute is false; otherwise returns
    true (if the attribute is true or is a function or expression).

    \sa isDesignable(), isStored()
*/
bool MetaProperty::isScriptable(const TypeObject *object) const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    bool b = flags & Scriptable;
    if (object) {
        void *argv[] = { &b };
        MetaObject::metacall(const_cast<TypeObject*>(object), MetaObject::QueryPropertyScriptable,
                              idx /*+ mobj->propertyOffset()*/, argv);
    }
    return b;
}

/*!
    Returns \c true if the property is stored for \a object; otherwise returns
    false.

    If no \a object is given, the function returns \c false if the
    \c{Q_PROPERTY()}'s \c STORED attribute is false; otherwise returns
    true (if the attribute is true or is a function or expression).

    \sa isDesignable(), isScriptable()
*/
bool MetaProperty::isStored(const TypeObject *object) const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    bool b = flags & Stored;
    if (object) {
        void *argv[] = { &b };
        MetaObject::metacall(const_cast<TypeObject*>(object), MetaObject::QueryPropertyStored,
                              idx /*+ mobj->propertyOffset()*/, argv);
    }
    return b;
}

/*!
    Returns \c true if this property is designated as the \c USER
    property, i.e., the one that the user can edit for \a object or
    that is significant in some other way.  Otherwise it returns
    false. e.g., the \c text property is the \c USER editable property
    of a QLineEdit.

    If \a object is null, the function returns \c false if the \c
    {Q_PROPERTY()}'s \c USER attribute is false. Otherwise it returns
    true.

    \sa MetaObject::userProperty(), isDesignable(), isScriptable()
*/
bool MetaProperty::isUser(const TypeObject *object) const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    bool b = flags & User;
    if (object) {
        void *argv[] = { &b };
        MetaObject::metacall(const_cast<TypeObject*>(object), MetaObject::QueryPropertyUser,
                              idx /*+ mobj->propertyOffset()*/, argv);
    }
    return b;
}

/*!
    \since 4.6
    Returns \c true if the property is constant; otherwise returns \c false.

    A property is constant if the \c{Q_PROPERTY()}'s \c CONSTANT attribute
    is set.
*/
bool MetaProperty::isConstant() const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    return flags & Constant;
}

/*!
    \since 4.6
    Returns \c true if the property is static; otherwise returns \c false.

    A property is static if the \c{Q_PROPERTY()}'s \c STATIC attribute
    is set.
*/
bool MetaProperty::isStatic() const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    return flags & Static;
}

bool MetaProperty::isPointer() const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    return flags & Pointer;
}

/*!
    \since 4.6
    Returns \c true if the property is final; otherwise returns \c false.

    A property is final if the \c{Q_PROPERTY()}'s \c FINAL attribute
    is set.
*/
bool MetaProperty::isFinal() const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    return flags & Final;
}

/*!
    \obsolete

    Returns \c true if the property is editable for the given \a object;
    otherwise returns \c false.

    If no \a object is given, the function returns \c false if the
    \c{Q_PROPERTY()}'s \c EDITABLE attribute is false; otherwise returns
    true (if the attribute is true or is a function or expression).

    \sa isDesignable(), isScriptable(), isStored()
*/
bool MetaProperty::isEditable(const TypeObject *object) const
{
    if (!mobj)
        return false;
    int flags = KEY_DATA(mobj, handle + 2);
    bool b = flags & Editable;
    if (object) {
        void *argv[] = { &b };
        MetaObject::metacall(const_cast<TypeObject*>(object)
            , MetaObject::QueryPropertyEditable
            , idx /*+ mobj->propertyOffset()*/, argv);
    }
    return b;
}

/*!
    \class MetaClassInfo
    \inmodule QtCore

    \brief The MetaClassInfo class provides additional information
    about a class.

    \ingroup objectmodel

    Class information items are simple \e{name}--\e{value} pairs that
    are specified using Q_CLASSINFO() in the source code. The
    information can be retrieved using name() and value(). For example:

    \snippet code/src_corelib_kernel_qmetaobject.cpp 5

    This mechanism is free for you to use in your Qt applications. Qt
    doesn't use it for any of its classes.

    \sa MetaObject
*/


/*!
    \fn MetaClassInfo::MetaClassInfo()
    \internal
*/

/*!
    \fn const MetaObject *MetaClassInfo::enclosingMetaObject() const
    \internal
*/

/*!
    Returns the name of this item.

    \sa value()
*/
const char *MetaClassInfo::name() const
{
    if (!mobj)
        return 0;
    return rawStringData(mobj, KEY_DATA(mobj, handle));
}

/*!
    Returns the value of this item.

    \sa name()
*/
const char* MetaClassInfo::value() const
{
    if (!mobj)
        return 0;
    return rawStringData(mobj, KEY_DATA(mobj, handle + 1));
}

/*!
    Returns the name of this item.

    \sa value()
*/
const char *MetaPropertyInfo::name() const
{
    if (!mobj)
        return 0;
    return rawStringData(mobj, KEY_DATA(mobj, handle));
}

/*!
    Returns the value of this item.

    \sa name()
*/
const char* MetaPropertyInfo::value() const
{
    if (!mobj)
        return 0;
    return rawStringData(mobj, KEY_DATA(mobj, handle + 1));
}


CIM_END_NAMESPACE
