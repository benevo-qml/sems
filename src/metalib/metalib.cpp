#include "metalib.h"
#include "typeobjectdefs.h"

#include <QCoreApplication>


class MetalibData {
public:
    ~MetalibData() {}

    QString m_appDirPath;
};

Metalib *Metalib::m_instance = 0;

Metalib::Metalib()
: d_ptr(new MetalibData)
{
}

Metalib::~Metalib()
{
    DELETE_POINTER(d_ptr);
}

Metalib *Metalib::instance()
{
    if(NULL == m_instance)
    {
        m_instance = new Metalib;
    }

    return m_instance;
}

void Metalib::release()
{
    DELETE_POINTER(m_instance);
}

void Metalib::init()
{
    qLoadAllMetaObjects();
}

QString Metalib::applicationDirPath()
{
//    return instance()->d_ptr->m_appDirPath;
    return QCoreApplication::applicationDirPath();
}
