#include "RdfPropertyModel.h"

#include "metalib/corelib/typeobject.h"
#include "metalib/corelib/typeobjectdefs.h"
#include "metalib/corelib/metaobject.h"

RdfPropertyModel::RdfPropertyModel(QObject *parent) :
    QStandardItemModel(parent)
{

    m_roleNames[Role_RdfName] = "role_name";
    m_roleNames[Role_RdfValue] = "role_value";

//    addEntry( "Option A", "Recommended", "This is Option A" );
//    addEntry( "Option B", "Recommended", "This is Option B" );
//    addEntry( "Option C", "Recommended", "This is Option C" );
//    addEntry( "Option D", "Recommended", "This is Option D" );

//    addEntry( "Option E", "Optional", "This is Option E" );
//    addEntry( "Option F", "Optional", "This is Option F" );
//    addEntry( "Option G", "Optional", "This is Option G" );
//    addEntry( "Option H", "Optional", "This is Option H" );
}

void RdfPropertyModel::addEntry( const QString& name, const QString& type, const QString& description)
{
    QStandardItem *childEntry = new QStandardItem( name );
    childEntry->setData( description, Role_RdfValue );

    QStandardItem* entry = getBranch( type );
    entry->appendRow( childEntry );
}

QStandardItem *RdfPropertyModel::getBranch(const QString &branchName)
{
    QStandardItem* entry;
    QList<QStandardItem*> entries = this->findItems( branchName );
    if ( entries.count() > 0 )
    {
        entry = entries.at(0);
    }
    else
    {
        entry = new QStandardItem( branchName );
        entry->setData(branchName + "hh", Role_RdfValue);
        this->appendRow( entry );
    }
    return entry;
}

QHash<int, QByteArray> RdfPropertyModel::roleNames() const
{
    return m_roleNames;
}

bool RdfPropertyModel::retrieveNewModel(QString strClassName)
{
    clear();

    if (!qLoadMetaObject(strClassName.trimmed().toStdString().data(), &TypeObject::_staticMetaObject))
        return false;

    MetaObject &mo = TypeObject::_staticMetaObject;

    QStandardItem* item = 0;
    QStandardItem* subItem = 0;
    QStandardItem* subItem0 = 0;
    QString name, value;
    int nCount = 0;

    // Rdf Class
    nCount = mo.classInfoCount();
    name = "CLASS DEFINITION" + QString(" (%1)").arg(nCount);
    item = new QStandardItem(name);
    item->setData(name, Qt::DisplayRole);
    appendRow(item);

    // Append Class Info
    for (int i = 0; i < nCount; ++i){
        MetaClassInfo info = mo.classInfo(i);

        name = info.name();
        subItem = new QStandardItem(name);
        subItem->setData(name, Role_RdfName);

        value = info.value();
        subItem->setData(value, Role_RdfValue);

        item->appendRow(subItem);
    }
    item = subItem = 0;
    name = "";

    // RDF Properties
    nCount = mo.propertyCount();
    name = "PROPERTIES" + QString(" (%1)").arg(nCount);
    item = new QStandardItem(name);
    item->setData(name, Qt::DisplayRole);
    appendRow(item);

    for (int i = 0; i < nCount; ++i){
        MetaProperty prop = mo.property(i);

        int nPropInfoCount = prop.propertyInfoCount();
        name = prop.name() + QString(" (%1)").arg(nPropInfoCount);
        subItem = new QStandardItem(name);
        subItem->setData(name, Role_RdfName);

        value = prop.value().toString();
        subItem->setData(value, Role_RdfValue);

        item->appendRow(subItem);

        for (int j = 0; j < nPropInfoCount; ++j){
            MetaPropertyInfo propInfo = prop.propertyInfo(j);

            name = propInfo.name();
            subItem0 = new QStandardItem(name);
            subItem0->setData(name, Role_RdfName);

            value = propInfo.value();
            subItem0->setData(value, Role_RdfValue);

            subItem->appendRow(subItem0);
        }
    }

    return true;
}
