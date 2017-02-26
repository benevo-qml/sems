#include <QCoreApplication>

#include <qsqldatabase.h>

#include "rdfparser.h"

#include <iostream>
using namespace std;

const char path_rdf[] = "D:/devel/Studio/SEMS/app/data/cim/rdf/cim_schema.rdf";
const char path_db[] = "D:/devel/Studio/SEMS/app/data/cim/metalib.db";
const char path_cpright[] = "D:/devel/Studio/SEMS/app/data/cim/rdf/cim_desc.txt";
const char out_dir[] = "D:/devel/Studio/SEMS/src/cim";

void mocMetaDatum()
{
    RDFParser parse;

    double progress = 0;
    if (parse.mocMetaDatum(path_rdf, path_db, out_dir, progress))
    {
        cout << "\nOperation -> Moc Meta Datum : Complete ! \n\n";
    }else{
        cout << "\nOperation Failed ! \n\n";
    }

}

void splitRDF()
{
    RDFParser parse;

    double progress = 0;
    if (parse.splitRDF(path_rdf, out_dir, progress))
    {
        cout << "\nOperation -> Split RDF : Complete ! \n\n";
    }else{
        cout << "\nOperation Failed ! \n\n";
    }
}

void codeCppSource()
{
    RDFParser parse;

    double progress = 0;
    parse.setOutFolderName("core");
    if (parse.code2CppSource(path_rdf, path_cpright, out_dir, progress))
    {
        cout << "\nOperation -> Code Cpp Source : Complete ! \n\n";
    }else{
        cout << "\nOperation Failed ! \n\n";
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cout << "ooo started !\n------------\n\n\n";

//    mocMetaDatum();

//    splitRDF();

    codeCppSource();

    cout << "\n\n------------\nooo finished !\n";

    return a.exec();
}

