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

#ifndef CIMSCHMPARSER_P_H
#define CIMSCHMPARSER_P_H

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

#include "RDFParser.h"

#include "Markup.h"

#include <qlist.h>
#include <qscopedpointer.h>
#include <QXmlStreamReader>
#include <QMultiMap>

PARSE_BEGIN_NAMESPACE


class QVariant;
struct PARSE_EXPORT CIM_RDF
{
	struct RDF_HEAD
	{
		QString declaration;
		QString content;

		QMap<QString, QString> xmlns;
	};

	struct RDF_PROPERTY
	{
		QString rdf_content;

		QString name;
		QString dataType;

		QMap<QString, QString> info;
	};

	struct RDF_CLASS
	{
		QString rdf_content;

		QString name;
		QString supper;

		QMap<QString, QString> info;
		QMap<QString, RDF_PROPERTY*> props;
	};

	RDF_HEAD head;
	QMap<QString, RDF_CLASS *> mapClass;
	QMultiMap<QString, RDF_PROPERTY*> mapTempProp;   // 由于类的属性定义可能出现于类定义之前，故需要临时保留属性链表
	QMap<QString, RDF_PROPERTY*> mapRoleProp;		// 对于inverseRoleName不为为空的属性的rdf:ID与其指针的映射

	void Clean();
	void BuildRoleMap();

	~CIM_RDF()
	{
		Clean();
	}
};

// RDFParserPrivate:
class PARSE_EXPORT RDFParserPrivate
{
	Q_DECLARE_PUBLIC(RDFParser)

public:
	RDFParser *q_ptr;
	CIM_RDF rdf;

    RDFParserPrivate();
	~RDFParserPrivate(){}

	bool code2CppSource(const char *path_rdf, 
        const char *path_copyright, const char *out_dir, double& progress);
    bool splitRDF(const char *path_rdf, const char *out_dir, double& progress);
    bool mocMetaDatum(const char *path_rdf, const char *path_db, double &progress, const char *out_dir = NULL);

	QXmlStreamReader &GetXSRXml() { return m_XSRXml; }
	CMarkup &GetMKXml() { return m_MarkupXml; }
    QString findPSRParentClassName() const;
    QString findRootClassName(QString className) const;

    void setOutFolderName(const QString &name) { m_outFolderName = name; }

private:
	bool parse_CIM_RDF(const char *str);

	bool code_class_header(const char *dir, 
		const CIM_RDF::RDF_CLASS &value, const char *strTemplate);
	bool code_class_source(const char *dir,
		const CIM_RDF::RDF_CLASS &value, const char *strTemplate);
	bool code_class_header_p(const char *dir,
		const CIM_RDF::RDF_CLASS &value, const char *strTemplate);

	bool codeCPP_RootClass_Header(const char *dir,
		const CIM_RDF::RDF_CLASS &value, const char *strTemplate);
	bool codeCPP_RootClass_Source(const char *dir,
		const CIM_RDF::RDF_CLASS &value, const char *strTemplate);
	bool codeCPP_RootClass_Header_Private(const char *dir,
		const CIM_RDF::RDF_CLASS &value, const char *strTemplate);

private:
	QXmlStreamReader m_XSRXml;
	CMarkup m_MarkupXml;
    QString m_outFolderName;

};


PARSE_END_NAMESPACE

#endif // CIMSCHMPARSER_P_H
