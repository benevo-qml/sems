#ifndef METALIB_H
#define METALIB_H

#include "metalib_global.h"

class MetalibData;
class METALIB_EXPORT Metalib
{

public:
    explicit Metalib();
    ~Metalib();

    static Metalib *instance();
    static void release();


    static void init();
    static QString applicationDirPath();

private:
    static Metalib *m_instance;

public:
    MetalibData* d_ptr;

};

#endif // METALIB_H
