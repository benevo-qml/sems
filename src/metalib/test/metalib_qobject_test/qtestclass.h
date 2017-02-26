#ifndef QTESTCLASS_H
#define QTESTCLASS_H

#include <QObject>
#include "typeobject.h"

class QTestClass : public QObject, public TypeObject
{
    Q_OBJECT
public:
    explicit QTestClass(QObject *parent = 0);

signals:

public slots:
};

typedef QList<QTestClass *> QTestClassList;

//Q_DECLARE_METATYPE(QTestClass)
//Q_DECLARE_METATYPE(QTestClass*)

#endif // QTESTCLASS_H
