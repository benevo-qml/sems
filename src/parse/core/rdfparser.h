/****************************************************************************
**
** Copyright (C) 2015 The oOo Company Ltd.
** Contact: http://www.oOo.io/licensing/
**
** This file is part of the Parse module of the SEMS Toolkit.
**
** $CIM_BEGIN_LICENSE:LGPL21$
**
** LICENSE INFO ABOUT ADD HERE...
**
** $CIM_END_LICENSE$
**
****************************************************************************/

#ifndef RDFPARSER_H
#define RDFPARSER_H

#include "../parse_global.h"
#include <qscopedpointer.h>

PARSE_BEGIN_NAMESPACE

class RDFParserPrivate;


/**
* 解析符合IEC 61970的各类Schema模型数据，比如：RDF、XMI
* 文件载体为CIM RDF（W3C RDF的一个子集）规范文件.
*/
class PARSE_EXPORT RDFParser
{
public:
    explicit RDFParser();
    virtual ~RDFParser();

    bool code2CppSource(const char *path_rdf, const char *path_copyright, const char *out_dir, double&progress);
    bool mocMetaDatum(const char *path_rdf, const char *path_db, const char *out_dir, double &progress);
    bool splitRDF(const char *path_rdf, const char *out_dir, double&progress);

    void setOutFolderName(const QString &name);

protected:
    RDFParser(RDFParserPrivate &dd);

protected:
    QScopedPointer<RDFParserPrivate> d_ptr;

private:
//    Q_DISABLE_COPY(RDFParser)
    Q_DECLARE_PRIVATE(RDFParser)

};


PARSE_END_NAMESPACE

#endif // RDFPARSER_H
