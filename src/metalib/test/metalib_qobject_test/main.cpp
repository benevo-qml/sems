#include <QCoreApplication>

#include "typeobject.h"
#include "typeobjectdefs.h"
#include "metaobject.h"
#include "naming.h"

#include "qtestclass.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cout << "ooo started !\n------------\n\n\n";

    Naming obj;

    cout << obj.metaObject()->propertyCount() << endl;
    cout << obj.cim_metaObject()->propertyCount() << endl;

    MetaProperty prop = obj.cim_metaObject()->property(0);
    cout << "   - Property 0 : " << prop.name() << "= [ "
        << prop.typeName() << ", "
        << (prop.value().isNull() ? "NULL" : prop.value().toByteArray().data()) << " ]." << endl;

    prop.setValue(QVariant(false));
    cout << "   + Assign as bool value 'false', then : [ "
        << prop.typeName() << ", "
        << (prop.value().isNull() ? "NULL" : prop.value().toByteArray().data()) << " ]." << endl;

    cout << "\n\n------------\nooo finished !\n";

    return a.exec();
}

