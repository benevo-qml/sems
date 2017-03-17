/****************************************************************************
**
** Copyright (C) 2015 The oOo Company Ltd.
** Contact: http://www.oOo.io/licensing/
**
** This file is part of the CIM module of the SEMS Toolkit.
**
** $CIM_BEGIN_LICENSE:LGPL21$
**
** LICENSE INFO ABOUT ADD HERE...
**
** $CIM_END_LICENSE$
**
****************************************************************************/

#ifndef QQuickTypeObject_H
#define QQuickTypeObject_H

#include <QObject>

#include "metalib/corelib/typeobject.h"
#include "metalib/corelib/metaobject.h"

CIM_BEGIN_NAMESPACE


class QQuickTypeObjectPrivate;
class PSRType;
class Measurement;
class LogMe;

typedef QList<Measurement *> MeasurementPtrList;

/**
 * A power system resource can be an item of equipment such as a switch, an
 * equipment container containing many individual items of equipment such as a
 * substation, or an organisational entity such as sub-control area. Power system
 * resources can have measurements associated.
 */
class QQuickTypeObject : public QObject, public TypeObject
{
//    CIM_OBJECT
    Q_OBJECT

public:
    Q_PROPERTY(double progress MEMBER m_progress NOTIFY progressChanged)

signals:
    void progressChanged();

public:
    explicit QQuickTypeObject(QObject *parent = 0);
    explicit QQuickTypeObject(const QQuickTypeObject &);
//    QQuickTypeObject& operator =(const QQuickTypeObject &);
    virtual ~QQuickTypeObject();

    Q_INVOKABLE QString className() const;
    Q_INVOKABLE int cim_classInfoCount() const;
    Q_INVOKABLE QString cim_classInfoName(int idx) const;
    Q_INVOKABLE QString cim_classInfoValue(int idx) const;

    Q_INVOKABLE int cim_propertyCount() const;
    Q_INVOKABLE int cim_propertyInfoCount(int idx) const;
    Q_INVOKABLE QString cim_propertyName(int idx) const;
    Q_INVOKABLE QString cim_propertyValue(int idx) const;

    Q_INVOKABLE QString cim_propertyInfoName(int idx_prop, int idx_info) const;
    Q_INVOKABLE QString cim_propertyInfoValue(int idx_prop, int idx_info) const;

    Q_INVOKABLE bool cim_loadMetaObject(QString strClassName);
    Q_INVOKABLE void startElapseTimer();
    Q_INVOKABLE qint64 elapsed() const;

    Q_INVOKABLE bool runRdfTask(int type, QString path_rdf, QString path_db, QString out_dir);
    Q_INVOKABLE bool isRdfExist(QString path_rdf);
    Q_INVOKABLE QString appDirPath();
    Q_INVOKABLE QString metalib_dbPath();

protected:
//    QQuickTypeObject(QQuickTypeObjectPrivate &dd, QQuickTypeObject* parent = 0);

public:

private:
//    Q_DISABLE_COPY(QQuickTypeObject)
//    Q_DECLARE_PRIVATE(QQuickTypeObject)

private:
    LogMe* _logger;
    double m_progress;
};

typedef QList<QQuickTypeObject *> PSRList;

//Q_DECLARE_METATYPE(QQuickTypeObject)
//Q_DECLARE_METATYPE(QQuickTypeObject*)

CIM_END_NAMESPACE

#endif // QQuickTypeObject_H
