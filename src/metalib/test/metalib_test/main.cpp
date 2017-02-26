#include <QCoreApplication>

#include "typeobject.h"
#include "naming.h"

#include <qsqldatabase.h>
#include "metaobject.h"
#include "metalib.h"
#include <QDir>

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Metalib::init();

    cout << "ooo started !\n------------\n\n\n";

    cout << "Naming: MetaObject::createTypeInstance: \n";
    QVariant var = MetaObject::createTypeInstance("Naming");
    Q_ASSERT(var.isValid());
    cout << var.typeName() << endl;
    const Naming &obj = var.value<Naming>();
    cout << obj.cim_metaObject()->propertyCount() << endl;
    cout << "m_ruid = " << obj.ruid().toUpper().toStdString() << endl;

    MetaProperty prop = obj.cim_metaObject()->property(0);
    cout << "   - Property 0 : " << prop.name() << "= [ "
        << prop.typeName() << ", "
        << (prop.value().isNull() ? "NULL" : prop.value().toByteArray().data()) << " ]." << endl;

    prop.setValue(QVariant(false));
    cout << "   + Assign as bool value 'false', then : [ "
        << prop.typeName() << ", "
        << (prop.value().isNull() ? "NULL" : prop.value().toByteArray().data()) << " ]." << endl;

    prop.setValue(QVariant(11.2));
    cout << "   + Assign as double value '11.2', then : [ "
        << prop.typeName() << ", "
        << (prop.value().isNull() ? "NULL" : prop.value().toByteArray().data()) << " ]." << endl;

    prop.setValue(QVariant(QByteArray("hello world")));
    cout << "   + Assign as QByteArray value 'hello world', then : [ "
        << prop.typeName() << ", "
        << (prop.value().isNull() ? "NULL" : prop.value().toByteArray().data()) << " ]." << endl;

    prop.setValue(QVariant(QString("hello world")));
    cout << "   + Assign as QString value 'hello world', then : [ "
        << prop.typeName() << ", "
        << (prop.value().isNull() ? "NULL" : prop.value().toByteArray().data()) << " ]." << endl;

    obj.cim_metaObject()->print("D:/");

    cout << "\n\nNaming: Naming(): \n";
    Naming obj1;
    cout << obj1.cim_metaObject()->propertyCount() << endl;
    cout << "ruid = " << obj1.ruid().toUpper().toStdString() << endl;

    cout << "\n\n------------\nooo finished !\n";

    return a.exec();
}

