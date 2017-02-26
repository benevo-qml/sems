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

#include "rdfparser.h"
#include "private/rdfparser_p.h"

#include <qdir.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qsqlerror.h>
#include <qsqlrecord.h>
#include <qsqlresult.h>
#include <QDebug>
#include <iostream>

#include "parse_global.h"

#ifdef USE_METALIB_LIB
# include "typeobjectdefs.h"
#endif

#include <sstream>
typedef std::basic_stringstream<char> stringstream;

#include <iostream>

#define LOWER_FIRST_CHAR(str) str.toLower().left(1) + str.right(str.length() - 1)
#define CASE_FIRST_CHAR(str) str.toUpper().left(1) + str.right(str.length() - 1)

#if defined(MARKUP_WCHAR)
    #define _Q(x) QString::fromStdWString(x)
    #define _T(x)   L ##x
#else
    #define _Q(x) QString::fromStdString(x)
    #define _T(x)   x
#endif

#define LEN_PROPERTY_FIELD 7

bool deleteDir(const QString &dirName)
{
	QDir directory(dirName);
	if (!directory.exists())
	{
		return true;
	}

	QString srcPath = QDir::toNativeSeparators(dirName);
	if (!srcPath.endsWith(QDir::separator()))
		srcPath += QDir::separator();

	QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
	bool error = false;
	for (QStringList::size_type i = 0; i != fileNames.size(); ++i)
	{
		QString filePath = srcPath + fileNames.at(i);
		QFileInfo fileInfo(filePath);
		if (fileInfo.isFile() || fileInfo.isSymLink())
		{
			QFile::setPermissions(filePath, QFile::WriteOwner);
			if (!QFile::remove(filePath))
			{
				//qDebug() << "remove file" << filePath << " faild!";
				error = true;
			}
		}
		else if (fileInfo.isDir())
		{
			if (!deleteDir(filePath))
			{
				error = true;
			}
		}
	}

	if (!directory.rmdir(QDir::toNativeSeparators(directory.path())))
	{
		//qDebug() << "remove dir" << directory.path() << " faild!";
		error = true;
	}

	return !error;
}

void CIM_RDF::Clean()
{
	QMapIterator<QString, RDF_CLASS *> i(mapClass);
	while (i.hasNext())
	{
		i.next();

		qDeleteAll(i.value()->props);
		i.value()->props.clear();
	}
	qDeleteAll(mapClass);
	mapClass.clear();
}

void CIM_RDF::BuildRoleMap()
{
	mapRoleProp.clear();
	QMapIterator<QString, RDF_CLASS *> i(mapClass);
	while (i.hasNext())
	{
		i.next();

		QMapIterator<QString, RDF_PROPERTY*> j(i.value()->props);
		while (j.hasNext())
		{
			j.next();

			RDF_PROPERTY &varProp = *(j.value());
			const QString &rdfID = varProp.info.value(QString("rdf:ID"));
			if (varProp.info.contains(QString("cims:inverseRoleName")))
			{
				const QString inverseRoleName = varProp.info.value("cims:inverseRoleName").split("#").back();
				if (!inverseRoleName.isEmpty())
					mapRoleProp.insert(rdfID, j.value());
			}
		}
	}
}

RDFParserPrivate::RDFParserPrivate()
{
    m_outFolderName = "";
}

bool RDFParserPrivate::code2CppSource(const char *path_rdf,
    const char *path_copyright, const char *out_dir, double& progress)
{
    if (0 == path_rdf || !QFile::exists(path_rdf)
        || 0 == out_dir
        || 0 == path_copyright)
		return false;

    if (!parse_CIM_RDF(path_rdf) || rdf.mapClass.isEmpty())
		return false;

	// variables declare.
    QFile file(path_rdf);
	QTextStream out(&file);
    const QFileInfo fileInfo(path_rdf);
    QDir rootDir(QString(out_dir) + "");
    if (!m_outFolderName.isEmpty())
        rootDir.setPath(rootDir.path() + "/" + m_outFolderName);
    else
        rootDir.setPath(rootDir.path() + "/cpp_source");

	// create rootDir for source
    if (rootDir.exists(rootDir.path()))
		deleteDir(rootDir.path());
	rootDir.mkdir(rootDir.path());
	Q_ASSERT(rootDir.exists(rootDir.path()));

	// copyright context loaded from file cim_desc.txt
    const QString strTemplateFilePath(path_copyright);
	QString strTemplate;
	if (QFile::exists(strTemplateFilePath))
	{
		QFile fileRead(strTemplateFilePath);
		if (fileRead.open(QFile::ReadOnly | QFile::Text))
		{
			QTextStream in(&fileRead);
			strTemplate = in.readAll();

			fileRead.close();
		}
	}

	// generating source code for each class found.
	QMapIterator<QString, CIM_RDF::RDF_CLASS *> i(rdf.mapClass);
    progress = 0;
    int n = 0;
    int nLen = rdf.mapClass.count();
	while (i.hasNext())
	{
		i.next();

		const CIM_RDF::RDF_CLASS &varClass = *(i.value());
		
		const QString varClassName = varClass.name;
		const QString categoryName = varClass.info.value(QString("cims:belongsToCategory")).split("#").back();
		const QString categoryPath = rootDir.path().toLower() + "/" + categoryName.toLower();
		if (!rootDir.exists(categoryPath) && !categoryName.isEmpty())
		{
			rootDir.mkdir(categoryPath);
		}

		const QString privatePath = categoryPath.toLower() + "/private";
		if (!rootDir.exists(privatePath) && !rootDir.exists(privatePath))
		{
			rootDir.mkdir(privatePath);
		}

		// out name.h
        code_class_header(categoryPath.toUtf8().data(), varClass, strTemplate.toUtf8().data());

        if (!varClass.supper.isEmpty() ||
                (rdf.mapClass.contains("PowerSystemResource")
			&& varClassName == rdf.mapClass.value("PowerSystemResource")->supper))
		{
			// out className_p.h
			const QString strPrivateHeaderPath = categoryPath + "/private";
            code_class_header_p(strPrivateHeaderPath.toUtf8().data(), varClass, strTemplate.toUtf8().data());
		}

		// out name.cpp
        code_class_source(categoryPath.toUtf8().data(), varClass, strTemplate.toUtf8().data());

        progress = ( double(++n) / double(nLen));
	}

	return true;
}

bool RDFParserPrivate::splitRDF(const char *path_rdf, const char *out_dir, double &progress)
{
    if (0 == path_rdf || !QFile::exists(path_rdf))
		return false;

    if (!parse_CIM_RDF(path_rdf) || rdf.mapClass.isEmpty())
		return false;

	// variables declare.
    QFile file(path_rdf);
	QTextStream out(&file);
    const QFileInfo fileInfo(path_rdf);
    const QDir rootDir(QString(out_dir) + "/rdf_class");

	// create rootDir for source
	if (rootDir.exists(rootDir.path()))
		deleteDir(rootDir.path());
	rootDir.mkdir(rootDir.path());
	Q_ASSERT(rootDir.exists(rootDir.path()));

	// generating source code for each class found.
	QMapIterator<QString, CIM_RDF::RDF_CLASS *> i(rdf.mapClass);
    progress = 0;
    int n = 0;
    int nLen = rdf.mapClass.count();
    while (i.hasNext())
	{
		i.next();

		const CIM_RDF::RDF_CLASS &varClass = *(i.value());

		const QString newClassRdfFilePath = rootDir.path() + "/" + varClass.name + ".rdf";
		file.close();
		file.setFileName(newClassRdfFilePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			return false;
		}
		out.reset();
		out.setDevice(&file);

		out << rdf.head.declaration
			<< rdf.head.content;

		// write class part
		out << varClass.rdf_content;// << endl;

		QMapIterator<QString, CIM_RDF::RDF_PROPERTY*> j(i.value()->props);
		while (j.hasNext())
		{
			j.next();

			CIM_RDF::RDF_PROPERTY &prop = *(j.value());

			out << prop.rdf_content;// << endl;
		}

		out << "</rdf:RDF>";

		out.flush();
		file.close();

        progress = ( double(++n) / double(nLen));
	}

	return true;
}

bool RDFParserPrivate::mocMetaDatum(const char *path_rdf,
    const char *path_db, double &progress, const char *out_dir)
{
    if (0 == path_rdf || !QFile::exists(path_rdf))
		return false;

    if (!parse_CIM_RDF(path_rdf))
		return false;
	if (rdf.mapClass.isEmpty())
		return false;

    QDir dirMoc;
    if (0 != out_dir && strcmp(out_dir, "") != 0)
	{
        if (!dirMoc.exists(out_dir))
            dirMoc.mkdir(out_dir);
        dirMoc.setPath(out_dir);
    }else{
        QFileInfo info(path_rdf);
        QString out_dir1 = info.absoluteDir().path() + "/meta_class";
        if (dirMoc.exists(out_dir1))
            dirMoc.mkdir(out_dir1);
        dirMoc.setPath(out_dir1);
    }

    QSqlDatabase pdb;
    if (0 != path_db)
    {
        QString connName = "iYoona.db.meta";
		if (QSqlDatabase::contains(connName))
            pdb = QSqlDatabase::database(connName);
		else
            pdb = QSqlDatabase::addDatabase("QSQLITE", connName);
        Q_ASSERT(pdb.isValid());
        pdb.setDatabaseName(path_db);

        if (!pdb.open())
			return false;
	}

    int nSizeUpdate = 0;
    int nSizeInsert = 0;
	QMapIterator<QString, CIM_RDF::RDF_CLASS *> i(rdf.mapClass);
    progress = 0;
    int nFinishedCount = 0;
    int nLenProgress = rdf.mapClass.count();
	while (i.hasNext())
	{
		i.next();

		const QString &className = i.key();
		CIM_RDF::RDF_CLASS &daa = *(i.value());

		QByteArray stringdatabytes;
		QByteArray stringobjdatabytes;
		QByteArray keyBytes;
		QByteArray rdfBytes;

		int nClassName = 0;
		int nClassInfoData = 14;
		int nClassInfoCount = daa.info.size();
		int nPropCount = daa.props.size();
		int nPropData = nClassInfoData + nClassInfoCount * 2;

		keyBytes ooonrw(7, 3)
			ooonrw(nClassName, LEN_HANDLE)
			ooonrw(nClassInfoCount, LEN_COUNT) ooonrw(nClassInfoData, LEN_HANDLE)
			ooonrw(0, LEN_COUNT) ooonrw(0, LEN_HANDLE)
			ooonrw(nPropCount, LEN_COUNT) ooonrw(nPropData, LEN_HANDLE)
			ooonrw(0, LEN_COUNT) ooonrw(0, LEN_HANDLE)
			ooonrw(0, LEN_COUNT) ooonrw(0, LEN_HANDLE)
			ooonrw(0, LEN_FLAGS)
			ooonrw(0, LEN_COUNT);

		stringdatabytes ooo(className);

		rdfBytes oo(rdf.head.declaration.trimmed())
			oo(rdf.head.content.trimmed())
			oo(i.value()->rdf_content.trimmed());

		QMapIterator<QString, QString> j(daa.info);
		int n = 0;
		int m = 0;
		while (j.hasNext())
		{ // class info
			j.next();

			stringdatabytes ooo(j.key()) ooo(j.value());
			keyBytes ooon(n + 1) ooon(n + 2);

			n += 2;
		}

		//QMapIterator<QString, QMap<QString, QString> > k(daa.props);
		QMapIterator<QString, CIM_RDF::RDF_PROPERTY*> k(daa.props);
		int prevPropInfoCount = 0;
		int prevPropData = nPropData + LEN_PROPERTY_FIELD * nPropCount;
		QStringList propInfoLst;
		QByteArray propInfoKeyBytes;
		while (k.hasNext())
		{ // properties
			// properties: name, type, flags, info.count, info.pos, value.startpos, value.endpos
			k.next();

			int nSubPropData = prevPropData + 2 * prevPropInfoCount;
			prevPropInfoCount = k.value()->info.size();
			prevPropData = nSubPropData;

			stringdatabytes ooo(k.key()) ooo(k.value()->dataType);
			stringobjdatabytes ooo("");
			keyBytes ooon(n + 1)
				ooon(n + 2)
				ooonrw(0x00095003, 13)
				ooon(k.value()->info.size())
				ooon(nSubPropData)
				ooon(m)
				ooon(m);

			n += 2;
			m += 1;

			rdfBytes oo(k.value()->rdf_content.trimmed());

			QMapIterator<QString, QString> m(k.value()->info);
			while (m.hasNext())
			{ // the property's info
				m.next();

				stringdatabytes ooo(m.key()) ooo(m.value());
				propInfoKeyBytes ooon(n + 1) ooon(n + 2);

				n += 2;
			}
		}
		keyBytes oo(propInfoKeyBytes);
		rdfBytes oo("</rdf:RDF>");
		//keyBytes.remove(keyBytes.size() - 1, 1);
		//stringdatabytes.remove(stringdatabytes.size() - 1, 1);

#ifdef USE_METALIB_LIB
		if (!dirMoc.path().isEmpty())
		{
            MetaObject mo = {
                Q_0PTR, stringdatabytes, keyBytes,
                QByteArray("\0"), Q_0PTR, Q_0PTR, Q_0PTR,
                true//CIM_META_REGISTER(TypeObject) && CIM_META_LOAD(TypeObject)
            };

            mo.print(dirMoc.path().toUtf8().data());
		}
#endif

        QSqlQuery query(pdb);
        query.prepare("SELECT count(*) FROM tb_metaobject WHERE name = ?");
        query.addBindValue(className);
        if (query.exec())
        {
            query.next();
//            qDebug() << query.record().value(0).toInt();
            if (query.record().value(0).toInt() > 0)
            {
                query.prepare("UPDATE tb_metaobject SET supper = ?, stringdata = ?, key = ?, rdf = ?, objdata = ? WHERE name = ?");
                query.addBindValue(daa.supper);
                query.addBindValue(stringdatabytes);
                query.addBindValue(keyBytes);
                query.addBindValue(rdfBytes);
                query.addBindValue(stringobjdatabytes);
                query.addBindValue(className);

                nSizeUpdate++;
            }
            else
            {
                query.prepare("INSERT INTO tb_metaobject(name, supper, stringdata, key, objdata, rdf) values(?, ?, ?, ?, ?, ?)");
                query.addBindValue(className);
                query.addBindValue(daa.supper);
                query.addBindValue(stringdatabytes);
                query.addBindValue(keyBytes);
                query.addBindValue(stringobjdatabytes);
                query.addBindValue(rdfBytes);
                nSizeInsert++;
            }
            if (!query.exec())
            {
                qDebug() << "show last query is : " << qPrintable(query.lastQuery());
                qDebug() << "show last error is : " << qPrintable(query.lastError().text());

                query.finish();
                return false;
            }

//            query.exec("SELECT stringdata FROM tb_metaobject WHERE name = 'QuickTypeObject'");
//            if (query.next()){
//                QByteArray data = query.value(0).toByteArray();
//                qDebug() << "from db size: " << data.size();
//                qDebug() << "equal?" << (data == stringdatabytes);
//            }
        }
        query.finish();

        progress = ( double(++nFinishedCount) / double(nLenProgress));

        std::cout << QByteArray::number(nSizeUpdate).toStdString() << " updated, "
                 << QByteArray::number(nSizeInsert).toStdString() << " created!\r";
	}

    std::cout << "\n";

	return true;
}

QString RDFParserPrivate::findPSRParentClassName() const
{
    if (rdf.mapClass.contains("PowerSystemResource"))
        return rdf.mapClass.value("PowerSystemResource")->supper;
    else
        return "";
}

QString RDFParserPrivate::findRootClassName(QString className) const
{
    if (!rdf.mapClass.contains(className))
        return "";

    QString name = className;
    className = rdf.mapClass.value(className)->supper;
    if (className.isEmpty())
        return name;

    return findRootClassName(className);
}

bool RDFParserPrivate::parse_CIM_RDF(const char *path_rdf)
{
#define o_var(var) \
	<< #var << " : " << var
#define o_nop << "\n"
#define  _out qDebug()

    if (0 != path_rdf && !QFile::exists(path_rdf))
		return false;

	rdf.Clean();

	CMarkup &xml = GetMKXml();
    if (!xml.Load(QString(path_rdf).toStdWString()))
		return false;
	xml.ResetPos();

    size_t pos = xml.GetDoc().find(_T("<rdf:RDF"), 0);
	if (pos > 0)
        rdf.head.declaration = _Q(xml.GetDoc().substr(0, pos));

    size_t newPos = xml.GetDoc().find_first_of(_T("<"), pos + 1);
    rdf.head.content = _Q(xml.GetDoc().substr(pos, newPos - pos));

	int n = 0;
	QString sVal;

    while (xml.FindElem(MCD_CSTR(_T("rdf:RDF"))))
	{
        QString sName = _Q(xml.GetTagName());
        QString sValue = _Q(xml.GetData());
		QString sDoc;
		//_out o_var(sName) o_nop o_var(sValue);

		n = 0;
		while (1)
		{
            sName = _Q(xml.GetAttribName(n));
			if (sName.isEmpty())
				break;
            sValue = _Q(xml.GetAttrib(xml.GetAttribName(n)));
			rdf.head.xmlns.insert(sName, sValue);
			n++;
		}

		xml.IntoElem();
		while (xml.FindElem())
		{
            sName = _Q(xml.GetTagName());
            sValue = _Q(xml.GetData());
			//_out o_var(sName) o_nop o_var(sValue);

            sDoc = _Q(xml.GetSubDoc());
            //QString sChildDoc = _Q(xml.GetChildSubDoc());
            //QString sContent = _Q(xml.GetElemContent());
			//_out o_var(sDoc);
			//_out o_var(sChildDoc);
			//_out o_var(sContent);

			if (strcmp(sName.toUtf8().data(), "rdfs:Class") == 0)
			{ // Find a new Class
				CIM_RDF::RDF_CLASS *varClassPtr = new CIM_RDF::RDF_CLASS;
				CIM_RDF::RDF_CLASS &varClass = *varClassPtr;

				varClass.rdf_content = sDoc;

				// Doc info : <rdf:ID> <rdf:about>
				n = 0;
				while (1)
				{
                    sName = _Q(xml.GetAttribName(n));
					if (sName.isEmpty())
						break;
                    sValue = _Q(xml.GetAttrib(xml.GetAttribName(n)));
					varClass.info.insert(sName, sValue);
					n++;
				}

				// child element
				xml.IntoElem();
				while (xml.FindElem())
				{
                    sName = _Q(xml.GetTagName());
                    sValue = _Q(xml.GetData());
					//_out o_var(sName) o_nop o_var(sValue);

                    //QString sDoc = _Q(xml.GetSubDoc());
                    //QString sChildDoc = _Q(xml.GetChildSubDoc());
                    //QString sContent = _Q(xml.GetElemContent());
					//_out o_var(sDoc);
					//_out o_var(sChildDoc);
					//_out o_var(sContent);

					if (strcmp(sName.toUtf8().data(), "rdfs:subClassOf") == 0
						|| strcmp(sName.toUtf8().data(), "cims:belongsToCategory") == 0)
					{
                        sValue = _Q(xml.GetAttrib(xml.GetAttribName(n = 0)));
						if (strcmp(sName.toUtf8().data(), "rdfs:subClassOf") == 0)
							varClass.supper = sValue.split("#").back();
					}

					varClass.info.insert(sName, sValue);
				}
				xml.OutOfElem();

				if (varClass.info.contains(QString("rdf:ID")))
					varClass.name = varClass.info.value(QString("rdf:ID"));
				else if (varClass.info.contains(QString("rdfs:label")))
					varClass.name = varClass.info.value(QString("rdfs:label"));
				else
				{
					DELETE_POINTER(varClassPtr);
					continue;
				}

				if (!rdf.mapClass.contains(varClass.name))
				{
					rdf.mapClass.insert(varClass.name, varClassPtr);
				}
				else
				{
					DELETE_POINTER(varClassPtr);
					continue;
				}

			}
			else if (strcmp(sName.toUtf8().data(), "rdf:Property") == 0)
			{ // Find a new property
				CIM_RDF::RDF_PROPERTY *varPropPtr = new CIM_RDF::RDF_PROPERTY;
				CIM_RDF::RDF_PROPERTY &varProp = *varPropPtr;

				varProp.rdf_content = sDoc;

				// Doc info : <rdf:ID> <rdf:about>
				n = 0;
				while (1)
				{
                    sName = _Q(xml.GetAttribName(n));
					if (sName.isEmpty())
						break;
                    sValue = _Q(xml.GetAttrib(xml.GetAttribName(n)));
					varProp.info.insert(sName, sValue);
					n++;
				}

				// child element
				xml.IntoElem();
				while (xml.FindElem())
				{
                    sName = _Q(xml.GetTagName());
                    sValue = _Q(xml.GetData());
					//_out o_var(sName) o_nop o_var(sValue);

                    //QString sDoc = _Q(xml.GetSubDoc());
                    //QString sChildDoc = _Q(xml.GetChildSubDoc());
                    //QString sContent = _Q(xml.GetElemContent());
					//_out o_var(sDoc);
					//_out o_var(sChildDoc);
					//_out o_var(sContent);

					if (strcmp(sName.toUtf8().data(), "rdfs:domain") == 0
						|| strcmp(sName.toUtf8().data(), "rdfs:range") == 0
						|| strcmp(sName.toUtf8().data(), "cims:dataType") == 0
						|| strcmp(sName.toUtf8().data(), "cims:multiplicity") == 0
						|| strcmp(sName.toUtf8().data(), "cims:inverseRoleName") == 0)
					{
                        sValue = _Q(xml.GetAttrib(xml.GetAttribName(n = 0)));;
					}

					varProp.info.insert(sName, sValue);
				}
				xml.OutOfElem();

				if (varProp.info.contains(QString("rdf:ID")))
					varProp.name = varProp.info.value(QString("rdf:ID"));
				else if (varProp.info.contains(QString("rdfs:label")))
					varProp.name = varProp.info.value(QString("rdfs:label"));
				else
				{
					DELETE_POINTER(varPropPtr);  // no prop name found
					continue;
				}

				if (!varProp.info.contains(QString("rdfs:domain"))
					|| varProp.info.value(QString("rdfs:domain")).isEmpty())
				{
					DELETE_POINTER(varPropPtr);// no belonged class name found
					continue;
				}

				QString varClassName = varProp.info.value(QString("rdfs:domain")).split("#").back();
				if (!rdf.mapClass.contains(varClassName))
				{// the belonged class not found in rdf
					rdf.mapTempProp.insert(varClassName, varPropPtr);
					continue;
				}

				//if (varProp.info.contains("cims:dataType"))
				//	varProp.dataType = varProp.info.value(QString("cims:dataType")).split("#").back();
				//else if (varProp.info.contains("rdfs:range"))
				//	varProp.dataType = varProp.info.value(QString("rdfs:range")).split("#").back();

				QMap<QString, CIM_RDF::RDF_PROPERTY*> &_pros
					= rdf.mapClass.value(varClassName)->props;
				if (!_pros.contains(varProp.name))
				{
					_pros.insert(varProp.name, varPropPtr);
				}
				else
				{
					CIM_RDF::RDF_PROPERTY *ptr = _pros.value(varProp.name);
					DELETE_POINTER(ptr);
					_pros[varProp.name] = varPropPtr;
				}
			}
		}
		xml.OutOfElem();
	}

	QMapIterator<QString, CIM_RDF::RDF_PROPERTY*> i(rdf.mapTempProp);
	while (i.hasNext())
	{
		i.next();

		if (rdf.mapClass.contains(i.key()))
		{
			QMap<QString, CIM_RDF::RDF_PROPERTY*> &_pros
				= rdf.mapClass.value(i.key())->props;
			if (_pros.contains(i.value()->name))
			{
				CIM_RDF::RDF_PROPERTY *ptr = i.value();
				DELETE_POINTER(ptr);
			}
			else
			{
				_pros.insert(i.value()->name, i.value());
			}
		}
		else
		{
			CIM_RDF::RDF_PROPERTY *ptr = i.value();
			_out << "Warning : ( " << i.key() << " 's Property : "
				<< i.value()->name << " not added";
			DELETE_POINTER(ptr);
		}
	}
	rdf.mapTempProp.clear();

	rdf.BuildRoleMap();

	// decide property dataType
	QMapIterator<QString, CIM_RDF::RDF_CLASS *> t(rdf.mapClass);
	while (t.hasNext())
	{
		t.next();

		CIM_RDF::RDF_CLASS &varClass = *(t.value());

		QMapIterator<QString, CIM_RDF::RDF_PROPERTY*> j(varClass.props);
		while (j.hasNext())
		{
			j.next();

			CIM_RDF::RDF_PROPERTY &varProp = *(j.value());

			if (varProp.info.contains("rdfs:range"))
				varProp.dataType = varProp.info.value(QString("rdfs:range")).split("#").back();

			const QString dataType = varProp.info.value(QString("cims:dataType")).split("#").back();
			if (!dataType.isEmpty())
			{ // is a simple property.
				varProp.dataType = dataType;
				if (dataType == "String")
					varProp.dataType = "QString";
			}
			else
			{ // is a association property.
				const QString range = varProp.info.value(QString("rdfs:range")).split("#").back();
				if (!range.isEmpty())
					varProp.dataType = range + "Ptr";

				const QString inverseRoleName = varProp.info.value(QString("cims:inverseRoleName")).split("#").back();
				const QString &isAggregate = varProp.info.value(QString("cims:isAggregate"));
				QString inversPropIsAggregate;
				if (rdf.mapRoleProp.contains(inverseRoleName))
				{
					CIM_RDF::RDF_PROPERTY &inversProp = *(rdf.mapRoleProp.value(inverseRoleName));
					inversPropIsAggregate = inversProp.info.value(QString("cims:isAggregate"));
					if (strcmp(inversPropIsAggregate.toUtf8().data(), "true") == 0)
					{ // is a aggregate property
						varProp.dataType = range + "PtrLst";
					}
				}
			}
		}
	}

	return true;
}


/**
* output class.h
*/
bool RDFParserPrivate::code_class_header(const char *dirPath,
	const CIM_RDF::RDF_CLASS &varClass, const char *strTemplate)
{
    if (0 == dirPath)
		return false;

    const QString varClassName = varClass.name;
//	if (rdf.mapClass.contains("PowerSystemResource")
//		&& varClassName == rdf.mapClass.value("PowerSystemResource")->supper)
//	{
//		return codeCPP_RootClass_Header(dirPath, varClass, strTemplate);
//	}

	// variables declare.
    QString str[4];

	const QDir dir(dirPath);
	if (!dir.exists(dirPath))
		dir.mkdir(dirPath);
	Q_ASSERT(dir.exists(dir.path()));

	// create class header file : name.h
	const QString fileHeaderPath = QString(dirPath).toLower() + "/" + varClassName.toLower() + ".h";
	QFile file(fileHeaderPath);
	QTextStream out(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	// preparing properties string
	QMapIterator<QString, CIM_RDF::RDF_PROPERTY*> i(varClass.props);
	while (i.hasNext())
	{
		i.next();

		const CIM_RDF::RDF_PROPERTY &prop = *(i.value());

		QString propTypeName;
		QString propVarName;
		QString getterName;
		QString setterName;

        QString dataType = prop.info.value(QString("cims:dataType")).split("#").back();
		propTypeName = prop.dataType;
		if (!dataType.isEmpty())
		{ // is a local Property.
			propVarName = "m_" + LOWER_FIRST_CHAR(prop.name.split(".").back());
			getterName = LOWER_FIRST_CHAR(prop.name.split(".").back());
			setterName = "set" + CASE_FIRST_CHAR(prop.name.split(".").back());

            if (dataType == "Float"){
                dataType = "float";
                propTypeName = "float";
            }else if (dataType == "Integer"){
                dataType = "int";
                propTypeName = "int";
            }else if (dataType == "Name" || dataType == "Description"
                       || dataType == "String"){
                dataType = "QString";
                propTypeName = "QString";
            }else if( dataType == "Boolean"){
                dataType = "bool";
                propTypeName = "bool";
            }else
                if (!str[3].contains("\"" + dataType.toLower() + ".h"))
                    str[3] += "#include \"" + dataType.toLower() + ".h\"\n";
        }
        else
        { // is a association Property.
			const QString range = prop.info.value(QString("rdfs:range")).split("#").back();
			const QString &isAggregate = prop.info.value(QString("cims:isAggregate"));
			const QString inverseRoleName = prop.info.value(QString("cims:inverseRoleName")).split("#").back();

			propVarName = "m_" + LOWER_FIRST_CHAR(range);
			getterName = LOWER_FIRST_CHAR(range);
			setterName = "set" + CASE_FIRST_CHAR(range);

			QString inversPropIsAggregate;
			if (rdf.mapRoleProp.contains(inverseRoleName))
			{
				CIM_RDF::RDF_PROPERTY &inversProp = *(rdf.mapRoleProp.value(inverseRoleName));
				inversPropIsAggregate = inversProp.info.value(QString("cims:isAggregate"));
				if (strcmp(inversPropIsAggregate.toUtf8().data(), "true") == 0)
				{
					str[1] += "typedef QList<" + range + " *> " + range + "PtrLst;\n";

					propTypeName = range + "PtrLst";
					propVarName = "m_" + LOWER_FIRST_CHAR(range) + "s";
					getterName = LOWER_FIRST_CHAR(range) + "s";
					setterName = "set" + CASE_FIRST_CHAR(range) + "s";
				}
			}

			if (!inversPropIsAggregate.isEmpty())
			{
				;
			}
			else if (strcmp(isAggregate.toUtf8().data(), "true") == 0)
			{
                str[1] += "typedef " + range + "* " + range + "Ptr;\n";

                propVarName = "m_cnt" + range;
                getterName = "cnt" + range;
                setterName = "setCnt" + range;
			}
			else
			{				
				getterName = "";
				setterName = "";
				//const QString multiplicity = prop.info.value(QString("cims:multiplicity")).split("#").back();
				//if (strcmp(multiplicity.toUtf8().data(), "M:1") == 0
				//	|| strcmp(multiplicity.toUtf8().data(), "M:0..1") == 0)
				//{ }
				//else
				//{
				//	getterName = "";
				//	setterName = "";
				//}
			}

			if (!setterName.isEmpty() && !getterName.isEmpty())
			{
				str[0] += "class " + range + ";\n";
			}
		}

		if (!setterName.isEmpty() && !getterName.isEmpty())
		{
			const QString &comment = prop.info.value(QString("rdfs:comment"));
			str[2] += "\t/**\n\t* "
				+ comment.trimmed()
				+ "\n\t*/"
                + "\n\tQ_CIM_PROPERTY(private, " + propTypeName
				+ ", " + propVarName
				+ ", READ, " + getterName
				+ ", WRITE, " + setterName + ")\n";
		}
	}

    // write copyright template desc if not 0
    if (0 != strTemplate)
		out << strTemplate << endl;

	// write body
	out << "#ifndef " << varClassName.toUpper() << "_H" << endl
		<< "#define " << varClassName.toUpper() << "_H" << endl
		<< endl;

	if (!varClass.supper.isEmpty())
	{
		out << "#include \"" << varClass.supper.toLower() << ".h\"" << endl
            << endl;
    }else{
        out << "#include \"cim_global.h\"" << endl
            << endl;

        if (varClassName == findPSRParentClassName()){
            out << "#include <qlist.h>\n"
                << "#include <qscopedpointer.h>\n"
                << "\n#include \"typeobject.h\"\n"
                << endl;
        }
    }

    if (!str[3].isEmpty())
        out << str[3] << endl;

	out << "CIM_BEGIN_NAMESPACE" << endl
		<< endl
		<< endl;

	if (!str[0].isEmpty())
        out << str[0].trimmed() << endl;

    bool isCIMObject = (findRootClassName(varClassName) == findPSRParentClassName());
    if (isCIMObject)
        out << "class " << varClassName << "Private;" << endl
            << endl;

	if (!str[1].isEmpty())
		out << str[1].trimmed() << endl
		<< endl;

	const QString &class_comment = varClass.info.value(QString("rdfs:comment"));
	out << "/**" << endl
		<< " * " << class_comment << endl
		<< " */" << endl
        << "class CIM_EXPORT " << varClassName
        << (varClass.supper.isEmpty() ? "" : " : public ") << varClass.supper
        << (varClass.name == findPSRParentClassName() ? " : public TypeObject" : "")
        << endl
		<< "{" << endl
//        << "\tQ_CIM_OBJECT" << endl
        << endl;

	if (!str[2].isEmpty())
	{
		// write Properties prepared above:
		out << "\t" << str[2].trimmed() << endl
			<< endl;
	}

    QString rootClass = findPSRParentClassName();
    QString sig_constructor = "(" + rootClass +" *parent = 0)";
	// write Constructors.
	out << "public:" << endl
        << "\texplicit " << varClassName << (isCIMObject ? sig_constructor : "()")<< ";" << endl
		<< "\tvirtual ~" << varClassName << "();" << endl
        << endl;

    if (isCIMObject){
        out << "protected:" << endl
            << "\t" << varClassName << "(" << varClassName << "Private &dd, "
               + rootClass + " *parent = 0);" << endl
            << endl;
    }

//    out << "public:" << endl
//        << endl;

    if (isCIMObject)
    out	<< "private:" << endl
//		<< "\tQ_DISABLE_COPY(" << varClassName << ")" << endl
        << "\tQ_CIM_DECLARE_PRIVATE(" << varClassName << ")" << endl
		<< endl;

	out << "};" << endl
		<< endl;

//    out << "Q_CIM_DECLARE_METATYPE(" << varClassName << ");\n";
//    out << "Q_CIM_DECLARE_METATYPE(" << varClassName << "*);\n\n";
	if (varClassName == "PowerSystemResource")
	{
        out << "typedef QList<PowerSystemResource *> PSRPtrLst;" << endl
            << endl;
	}

    out << "typedef " << varClassName << "* " << varClassName << "Ptr;\n"
        << endl;

    out << endl << "CIM_END_NAMESPACE" << endl
		<< endl
		<< "#endif // " << varClassName.toUpper() << "_H" << endl;

	out.flush();
	file.close();

	return true;
}

/**
* output class.cpp
*/
bool RDFParserPrivate::code_class_source(const char *dirPath,
	const CIM_RDF::RDF_CLASS &varClass, const char *strTemplate)
{
    if (0 == dirPath)
		return false;

	const QString varClassName = varClass.name;
//	if (rdf.mapClass.contains("PowerSystemResource")
//		&& varClassName == rdf.mapClass.value("PowerSystemResource")->supper)
//	{
//		return codeCPP_RootClass_Source(dirPath, varClass, strTemplate);
//	}

	// variables declare.
	QString str;

	const QDir dir(dirPath);
	if (!dir.exists(dirPath))
		dir.mkdir(dirPath);
	Q_ASSERT(dir.exists(dir.path()));

	// create class header file : name.h
	const QString fileHeaderPath = QString(dirPath).toLower() + "/" + varClassName.toLower() + ".cpp";
	QFile file(fileHeaderPath);
	QTextStream out(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	// copyright written ahead.
    if (0 == !strTemplate)
		out << strTemplate << endl;

    bool isCIMObject = (findRootClassName(varClassName) == findPSRParentClassName());

	// write body
    out << "#include \"" << varClassName.toLower() << ".h\"" << endl;

    if (isCIMObject)
        out << "#include \"private/" << varClassName.toLower() << "_p.h\"" << endl;

    out << endl
		<< "CIM_BEGIN_NAMESPACE" << endl
		<< endl
        << endl;

    if (isCIMObject){
        out << varClassName << "Private::" << varClassName << "Private()" << endl
            << "{" << endl
            << "\tQ_CIM_Q(" << varClassName << ");" << endl
            << "\tQ_UNUSED(q);" << endl
            << "}" << endl
            << endl

            << "void " << varClassName << "Private::init()" << endl
            << "{" << endl
            << "\tQ_CIM_Q(" << varClassName << ");" << endl
            << "\tQ_UNUSED(q);" << endl
            << "}" << endl
            << endl

            << "/*!" << endl
            << "\tConstructs a(n) " << varClassName << " with a \\a parent." << endl
            << "*/" << endl
            << varClassName << "::" << varClassName << "(" << findPSRParentClassName() << " *parent)" << endl
            << "\t:" << (varClass.name == findPSRParentClassName() ? "TypeObject" : varClass.supper)
            << "(*new " << varClassName << "Private, "
            << (varClassName == findPSRParentClassName() ? "0" : "parent") << ")" << endl
            << "{" << endl
            << "\tQ_CIM_D(" << varClassName << ");" << endl
            << "\td->init();" << endl
            << "}" << endl
            << endl

            << "/*!" << endl
            << "\t\\internal" << endl
            << "*/" << endl
            << varClassName << "::" << varClassName << "(" << varClassName << "Private &dd, " << findPSRParentClassName() << " *parent)" << endl
            << "\t:" << (varClass.name == findPSRParentClassName() ? "TypeObject" : varClass.supper)
            << "(dd, " << (varClassName == findPSRParentClassName() ? "0" : "parent") << ")" << endl
            << "{" << endl
            << "\tQ_CIM_D(" << varClassName << ");" << endl
            << "\td->init();" << endl
            << "}" << endl
            << endl

            << "/*!" << endl
            << "\tDestroys a(n) " << varClassName << "." << endl
            << "*/" << endl
            << varClassName << "::~" << varClassName << "()" << endl
            << "{" << endl
            << "}" << endl
            << endl

            << endl
            << "CIM_END_NAMESPACE" << endl;
    }else {
        out << "/*!" << endl
            << "\tConstructs a(n) " << varClassName << " with a \\a parent." << endl
            << "*/" << endl
            << varClassName << "::" << varClassName << "()\n{" << endl
            << "}" << endl
            << endl

            << "/*!" << endl
            << "\tDestroys a(n) " << varClassName << "." << endl
            << "*/" << endl
            << varClassName << "::~" << varClassName << "()" << endl
            << "{" << endl
            << "}" << endl
            << endl

            << endl
            << "CIM_END_NAMESPACE" << endl;
    }

	out.flush();
	file.close();

	return true;
}

/**
* output class_p.h
*/
bool RDFParserPrivate::code_class_header_p(const char *dirPath,
	const CIM_RDF::RDF_CLASS &varClass, const char *strTemplate)
{
    if (0 == dirPath)
		return false;

	const QString varClassName = varClass.name;
//	if (rdf.mapClass.contains("PowerSystemResource")
//		&& varClassName == rdf.mapClass.value("PowerSystemResource")->supper)
//	{
//		return codeCPP_RootClass_Header_Private(dirPath, varClass, strTemplate);
//	}

	const QDir dir(dirPath);
	if (!dir.exists(dirPath))
		dir.mkdir(dirPath);
	Q_ASSERT(dir.exists(dir.path()));

	// create class header file : name.h
    const QString fileHeaderPath = QString(dirPath).toLower() + "/" + varClassName.toLower() + "_p.h";
	QFile file(fileHeaderPath);
	QTextStream out(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	// copyright written ahead.
    if (0 != strTemplate)
		out << strTemplate << endl;

	// write body
	out << "#ifndef " << varClassName.toUpper() << "_P_H" << endl
		<< "#define " << varClassName.toUpper() << "_P_H" << endl
		<< endl
		<< "//" << endl
		<< "//  W A R N I N G" << endl
		<< "//  -------------" << endl
		<< "//" << endl
		<< "// This file is not part of the SEMS API.  It exists for the convenience" << endl
		<< "// of _*.cpp.  This header file may change from version to version " << endl
		<< "// without notice, or even be removed." << endl
		<< "//" << endl
		<< "// We mean it." << endl
		<< "//" << endl
		<< endl
        << "#include \"../" << varClassName.toLower() << ".h\"" << endl
        << "#include \"private/" << (varClass.name == findPSRParentClassName() ? "typeobject" : varClass.supper.toLower()) << "_p.h\"" << endl
		<< endl
		<< "CIM_BEGIN_NAMESPACE" << endl
		<< endl
		<< endl
        << "class CIM_EXPORT " << varClassName << "Private : public " << (varClass.name == findPSRParentClassName() ? "TypeObject" : varClass.supper) << "Private" << endl
		<< "{" << endl;

    out << "\tQ_CIM_DECLARE_PUBLIC(" << varClassName << ")" << endl
		<< endl
		<< "public:" << endl
		<< "\texplicit " << varClassName << "Private();" << endl
		<< endl
		<< "\tinline void init();" << endl
		<< endl;

	out << "};" << endl
		<< endl
        << endl << "CIM_END_NAMESPACE" << endl
		<< endl
		<< "#endif // " << varClassName.toUpper() << "_P_H" << endl;

	out.flush();
	file.close();

	return true;
}

/**
* output root_class.h
*/
bool RDFParserPrivate::codeCPP_RootClass_Header(const char *dirPath,
	const CIM_RDF::RDF_CLASS &varClass, const char *strTemplate)
{
    if (0 == dirPath)
		return false;

	const QString varClassName = varClass.name;

	// variables declare.
	QString str[3];

	const QDir dir(dirPath);
	if (!dir.exists(dirPath))
		dir.mkdir(dirPath);
	Q_ASSERT(dir.exists(dir.path()));

	// create class header file : name.h
	const QString fileHeaderPath = QString(dirPath).toLower() + "/" + varClassName.toLower() + ".h";
	QFile file(fileHeaderPath);
	QTextStream out(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	// preparing properties string
	QMapIterator<QString, CIM_RDF::RDF_PROPERTY*> i(varClass.props);
	while (i.hasNext())
	{
		i.next();

		const CIM_RDF::RDF_PROPERTY &prop = *(i.value());

		QString propTypeName;
		QString propVarName;
		QString getterName;
		QString setterName;

		const QString dataType = prop.info.value(QString("cims:dataType")).split("#").back();
		if (!dataType.isEmpty())
		{ // is a local Property.
			propTypeName = dataType;
			propVarName = "m_" + LOWER_FIRST_CHAR(prop.name.split(".").back());
			getterName = LOWER_FIRST_CHAR(prop.name.split(".").back());
			setterName = "set" + CASE_FIRST_CHAR(prop.name.split(".").back());
		}
		else
		{ // is a association Property.
			const QString range = prop.info.value(QString("rdfs:range")).split("#").back();
			const QString &isAggregate = prop.info.value(QString("cims:isAggregate"));
			const QString inverseRoleName = prop.info.value(QString("cims:inverseRoleName")).split("#").back();

			propVarName = "m_" + LOWER_FIRST_CHAR(range);
			getterName = LOWER_FIRST_CHAR(range);
			setterName = "set" + CASE_FIRST_CHAR(range);

			QString inversPropIsAggregate;
			if (rdf.mapRoleProp.contains(inverseRoleName))
			{
				CIM_RDF::RDF_PROPERTY &inversProp = *(rdf.mapRoleProp.value(inverseRoleName));
				inversPropIsAggregate = inversProp.info.value(QString("cims:isAggregate"));
				if (strcmp(inversPropIsAggregate.toUtf8().data(), "true") == 0)
				{
					str[1] += "typedef QList<" + range + " *> " + range + "PtrLst;\n";

					propTypeName = range + "PtrLst";
					propVarName = "m_" + LOWER_FIRST_CHAR(range) + "s";
					getterName = LOWER_FIRST_CHAR(range) + "s";
					setterName = "set" + CASE_FIRST_CHAR(range) + "s";
				}
			}

			if (!inversPropIsAggregate.isEmpty())
			{
				;
			}
			else if (strcmp(isAggregate.toUtf8().data(), "true") == 0)
			{
				propVarName = "m_container";
				getterName = "container";
				setterName = "setContainer";
			}
			else
			{
				const QString multiplicity = prop.info.value(QString("cims:multiplicity")).split("#").back();
				if (strcmp(multiplicity.toUtf8().data(), "M:1") == 0
					|| strcmp(multiplicity.toUtf8().data(), "M:0..1") == 0)
				{
				}
				else
				{
					getterName = "";
					setterName = "";
				}
			}

			if (!setterName.isEmpty() && !getterName.isEmpty())
			{
				str[0] += "class " + range + ";\n";
			}
		}

		if (!setterName.isEmpty() && !getterName.isEmpty())
		{
			const QString &comment = prop.info.value(QString("rdfs:comment"));
			str[2] += "\t/**\n\t* "
				+ comment.trimmed()
				+ "\n\t*/"
                + "\n\tQ_CIM_PROPERTY(private, " + propTypeName
				+ ", " + propVarName
				+ ", READ, " + getterName
				+ ", WRITE, " + setterName + ")\n";
		}
	}

    // write copyright template desc if not 0
    if (0 == !strTemplate)
		out << strTemplate << endl;

	// write body
	out << "#ifndef " << varClassName.toUpper() << "_H" << endl
		<< "#define " << varClassName.toUpper() << "_H" << endl
		<< endl;

	out << "#include \"cim_global.h\"" << endl
		<< endl
		<< "#include <qlist.h>\n"
		<< "#include <qscopedpointer.h>\n"
		<< "\n"
		<< "#include \"" << varClassName.toLower() << "defs.h\"\n"
		<< endl;

	if (!varClass.supper.isEmpty())
	{
		out << "#include \"" << varClass.supper.toLower() << ".h\"" << endl
			<< endl;
	}

	out << "CIM_BEGIN_NAMESPACE" << endl
		<< endl
		<< endl;

	if (!str[0].isEmpty())
		out << str[0].trimmed() << endl;
	out << "class " << varClassName << ";" << endl;
	out << "class " << varClassName << "Private;" << endl
		<< endl;

	if (!str[1].isEmpty())
		out << str[1].trimmed() << endl
		<< endl;

	out << "typedef QList<" << varClassName << " *> " << varClassName << "PtrLst;\n\n";
	// rootData
	out << "class CIM_EXPORT " << varClassName << "Data\n"
		<< "{\n"
		<< "public:\n"
		<< "\tvirtual ~" << varClassName << "Data() = 0;\n"
		<< "\t" << varClassName << " *q_ptr;\n"
		<< "\t" << varClassName << " *parent;\n"
		<< "\t" << varClassName << "PtrLst children;\n\n"
		<< "\tuint isPSR : 1;\n"
		<< "\tuint wasDeleted : 1;\n"
		<< "\tuint isDeletingChildren : 1;\n"
		<< "\tuint unused : 25;\n"
		<< endl
		<< "};"
		<< endl;

	const QString &varClassComment = varClass.info.value(QString("rdfs:comment"));
	out << "/**" << endl
		<< " * " << varClassComment << endl
		<< " */" << endl
		<< "class CIM_EXPORT " << varClassName << (varClass.supper.isEmpty() ? "" : " : public ")
		<< varClass.supper << endl
		<< "{" << endl;

	if (!str[2].isEmpty())
	{
		// write Properties prepared above:
		out << "\t" << str[2].trimmed() << endl
			<< endl;
	}

	// write Constructors.
	out << "public:" << endl
		<< "\texplicit " << varClassName << "();" << endl
		<< "\tvirtual ~" << varClassName << "();" << endl
		<< endl
		<< "protected:" << endl
		<< "\t" << varClassName << "(" << varClassName << "Private &dd, " << varClassName << "* parent = 0);" << endl
		<< endl
		<< "protected:" << endl
		<< "\tQScopedPointer<" << varClassName << "Data> d_ptr;" << endl
		<< endl
		<< "private:" << endl
		<< "\tQ_DISABLE_COPY(" << varClassName << ")" << endl
        << "\tQ_CIM_DECLARE_PRIVATE(" << varClassName << ")" << endl
		<< endl;

	out << "};" << endl
		<< endl
		<< "CIM_END_NAMESPACE" << endl
		<< endl
		<< "#endif // " << varClassName.toUpper() << "_H" << endl;

	out.flush();
	file.close();

	return true;
}

/**
* output root_class_p.h
*/
bool RDFParserPrivate::codeCPP_RootClass_Header_Private(const char *dirPath,
	const CIM_RDF::RDF_CLASS &varClass, const char *strTemplate)
{
    if (0 == dirPath)
		return false;

	// variables declare.
	const QString varClassName = varClass.name;

	const QDir dir(dirPath);
	if (!dir.exists(dirPath))
		dir.mkdir(dirPath);
	Q_ASSERT(dir.exists(dir.path()));

	// create class header file : name.h
	const QString fileHeaderPath = QString(dirPath).toLower() + "/" + varClassName.toLower() + ".h";
	QFile file(fileHeaderPath);
	QTextStream out(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	// copyright written ahead.
    if (0 != strTemplate)
		out << strTemplate << endl;

	// write body
	out << "#ifndef " << varClassName.toUpper() << "_P_H" << endl
		<< "#define " << varClassName.toUpper() << "_P_H" << endl
		<< endl
		<< "//" << endl
		<< "//  W A R N I N G" << endl
		<< "//  -------------" << endl
		<< "//" << endl
		<< "// This file is not part of the SEMS API.  It exists for the convenience" << endl
		<< "// of _*.cpp.  This header file may change from version to version " << endl
		<< "// without notice, or even be removed." << endl
		<< "//" << endl
		<< "// We mean it." << endl
		<< "//" << endl
		<< endl
		<< "#include \"" << varClassName << ".h\"" << endl
		<< endl
		<< "CIM_BEGIN_NAMESPACE" << endl
		<< endl
		<< endl
		<< "class CIM_EXPORT " << varClassName << "Private : public " << varClassName << "Data" << endl
		<< "{" << endl;

    out << "\tQ_CIM_DECLARE_PUBLIC(" << varClassName << ")" << endl
		<< endl
		<< "public:" << endl
		<< "\texplicit " << varClassName << "Private();" << endl
		<< "\tvirtual ~" << varClassName << "Private();" << endl
		<< endl
		<< "\tinline void init(){}" << endl
		<< endl;

	out << "};" << endl
		<< endl
		<< "CIM_END_NAMESPACE" << endl
		<< endl
		<< "#endif // " << varClassName.toUpper() << "_P_H" << endl;

	out.flush();
	file.close();

	return true;
}

/**
* output root_source.cpp
*/
bool RDFParserPrivate::codeCPP_RootClass_Source(const char *dirPath,
	const CIM_RDF::RDF_CLASS &varClass, const char *strTemplate)
{
    if (0 == dirPath)
		return false;

	// variables declare.
	const QString varClassName = varClass.name;

	const QDir dir(dirPath);
	if (!dir.exists(dirPath))
		dir.mkdir(dirPath);
	Q_ASSERT(dir.exists(dir.path()));

	// create class header file : name.h
	const QString fileHeaderPath = QString(dirPath).toLower() + "/" + varClassName.toLower() + ".cpp";
	QFile file(fileHeaderPath);
	QTextStream out(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		return false;
	}

	// copyright written ahead.
    if (0 != strTemplate)
		out << strTemplate << endl;

	// write body
	out << "#include \"" << varClassName << ".h\"" << endl
		<< "#include \"private/" << varClassName << "_p.h\"" << endl
		<< endl
		<< "CIM_BEGIN_NAMESPACE" << endl
		<< endl
		<< endl

		<< varClassName << "Data::~" << varClassName << "Data() {}" << endl
		<< endl

		<< varClassName << "Private::" << varClassName << "Private()" << endl
		<< "{" << endl
		<< "\t// " << varClassName << "Data initialization" << endl
		<< "\tq_ptr = 0;" << endl
		<< "\tparent = 0;                                 // no parent yet. It is set by setParent()" << endl
		<< "\tisPSR = false;								// assume not a widget object" << endl
		<< "\twasDeleted = false;                         // double-delete catcher" << endl
		<< "\tisDeletingChildren = false;                 // set by deleteChildren()" << endl
		<< "}" << endl
		<< endl

		<< "void " << varClassName << "Private::init()" << endl
		<< "{" << endl
		<< "\tQ_Q(" << varClassName << ");" << endl
		<< "\tQ_UNUSED(q);" << endl
		<< "}" << endl
		<< endl

		<< "/*!" << endl
		<< "\tConstructs a(n) " << varClassName << endl
		<< "*/" << endl
		<< varClassName << "::" << varClassName << "()" << endl
		<< "\t:d_ptr(new " << varClassName << "Private)" << endl
		<< "{" << endl
        << "\tQ_CIM_D(" << varClassName << ");" << endl
		<< "\td_ptr->q_ptr = this;" << endl
		<< endl
		<< "\tQ_UNUSED(d);"
		<< "}" << endl
		<< endl

		<< "/*!" << endl
		<< "\t\\internal" << endl
		<< "*/" << endl
		<< varClassName << "::" << varClassName << "(" << varClassName << "Private &dd,  " << varClassName << " *parent)" << endl
		<< "\t:d_ptr(&dd)" << endl
		<< "{" << endl
        << "\tQ_CIM_D(" << varClassName << ");" << endl
		<< "\td_ptr->q_ptr = this;" << endl
		<< endl
		<< "\tQ_UNUSED(d);\n"
		<< "\tQ_UNUSED(parent);" << endl
		<< "}" << endl
		<< endl

		<< "/*!" << endl
		<< "\tDestroys a(n) " << varClassName << "." << endl
		<< "*/" << endl
		<< varClassName << "::~" << varClassName << "()" << endl
		<< "{" << endl
        << "\tQ_CIM_D(" << varClassName << ");" << endl
		<< "\td->wasDeleted = true;" << endl
		<< "}" << endl
		<< endl

		<< endl
		<< "CIM_END_NAMESPACE" << endl;

	out.flush();
	file.close();

	return true;
}



/*!
	Constructs an RDFParser.
*/
RDFParser::RDFParser()
: d_ptr(new RDFParserPrivate)
{
	Q_D(RDFParser);
	d_ptr->q_ptr = this;

	Q_UNUSED(d);
}

/*!
	\internal
*/
RDFParser::RDFParser(RDFParserPrivate &dd)
: d_ptr(&dd)
{
	Q_D(RDFParser);
	d_ptr->q_ptr = this;

	Q_UNUSED(d);
}

RDFParser::~RDFParser()
{
}

bool RDFParser::mocMetaDatum(const char *path_rdf, const char *path_db, const char *out_dir, double &progress)
{
	Q_D(RDFParser);

    return d->mocMetaDatum(path_rdf, path_db, progress, out_dir);
}

bool RDFParser::splitRDF(const char *path_rdf, const char *out_dir, double &progress)
{
	Q_D(RDFParser);

    return d->splitRDF(path_rdf, out_dir, progress);
}

bool RDFParser::code2CppSource(const char *path_rdf, const char *path_copyright, const char *out_dir, double&progress)
{
	Q_D(RDFParser);

    return d->code2CppSource(path_rdf, path_copyright, out_dir, progress);
}

void RDFParser::setOutFolderName(const QString &name)
{
    Q_D(RDFParser);

    d->setOutFolderName(name);
}
