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

#ifndef CIMPARSER_P_H
#define CIMPARSER_P_H

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

#include "CIMParser.h"

#include "Markup.h"
#include <qlist.h>
#include <qscopedpointer.h>
#include <QXmlStreamReader>

SEMS_BEGIN_NAMESPACE


class QVariant;

// CIMParserPrivate:
class PARSE_EXPORT CIMParserPrivate
{
	Q_DECLARE_PUBLIC(CIMParser)

public:
	CIMParser *q_ptr;

	~CIMParserPrivate()
	{
		CleanMem();
	}

	QXmlStreamReader &GetXSRXml()
	{
		return m_XSRXml;
	}

	CMarkup &GetMKXml()
	{
		return m_MarkupXml;
	}

	void CleanMem()
	{
		m_XSRXml.clear();
	}

private:
	/**
	* Xml 流解析
	*/
	QXmlStreamReader m_XSRXml;

	/**
	* Xml Markup解析
	*/
	CMarkup m_MarkupXml;

};


SEMS_END_NAMESPACE

#endif // CIMPARSER_P_H
