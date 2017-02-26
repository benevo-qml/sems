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

#include "qquicktypebbject.h"

#include <QThread>
#include <QElapsedTimer>
#include <QCoreApplication>
#include <QDir>

#include "itunes/core/logme.h"
#include "parse/core/rdfparser.h"

QElapsedTimer _timerElapsed;

/*!
Constructs a push button with no text and a \a parent.
*/

QQuickTypeObject::QQuickTypeObject(QObject *parent)
: QObject(parent), _logger(new LogMe(this))//, TypeObject(*new QQuickTypeObjectPrivate, 0)
{
    m_progress = 0;
//    m_timer = new QTimer();
//    m_timer->setInterval(200);
//    connect(m_timer, SIGNAL(timeout()), m_timer, SLOT(start()));
//    connect(m_timer, SIGNAL(timeout()), this, SLOT(progressChanged()));
//    m_timer->start();

//    Q_D(QQuickTypeObject);
//	d->init();
//    CIM_META_REGISTER(QQuickTypeObject) && CIM_META_LOAD(QQuickTypeObject);

	Q_UNUSED(parent);
}

QQuickTypeObject::QQuickTypeObject(const QQuickTypeObject &ot)
    : QObject(ot.parent()), TypeObject()
{
    m_progress = 0;
}

/*! 
	\internal
*/
//QQuickTypeObject::QQuickTypeObject(QQuickTypeObjectPrivate &dd, QQuickTypeObject *parent)
//: TypeObject(dd, parent)
//{
//    Q_D(QQuickTypeObject);
//	d->init();
//}

/*!
	Destroys the push button.
*/
QQuickTypeObject::~QQuickTypeObject()
{
    DELETE_POINTER(_logger);
//    DELETE_POINTER(m_timer);
}

QString QQuickTypeObject::className() const
{
    return cim_metaObject()->className();
}

int QQuickTypeObject::cim_classInfoCount() const
{
    return cim_metaObject()->classInfoCount();
}

QString QQuickTypeObject::cim_classInfoName(int idx) const
{
    return cim_metaObject()->classInfo(idx).name();
}

QString QQuickTypeObject::cim_classInfoValue(int idx) const
{
    return cim_metaObject()->classInfo(idx).value();
}

int QQuickTypeObject::cim_propertyCount() const
{
    return cim_metaObject()->propertyCount();
}

int QQuickTypeObject::cim_propertyInfoCount(int idx) const
{
    return cim_metaObject()->property(idx).propertyInfoCount();
}

QString QQuickTypeObject::cim_propertyName(int idx) const
{
    return cim_metaObject()->property(idx).name();
}

QString QQuickTypeObject::cim_propertyValue(int idx) const
{
    return cim_metaObject()->property(idx).value().toString();
}

QString QQuickTypeObject::cim_propertyInfoName(int idx_prop, int idx_info) const
{
    return cim_metaObject()->property(idx_prop).propertyInfo(idx_info).name();
}

QString QQuickTypeObject::cim_propertyInfoValue(int idx_prop, int idx_info) const
{
    return cim_metaObject()->property(idx_prop).propertyInfo(idx_info).value();
}

bool QQuickTypeObject::cim_loadMetaObject(QString strClassName)
{
    if (strClassName.isEmpty())
        return false;

    LogMe me(this);
    me.info("runing cim_loadMetaObject(TypeObject)");

    return qLoadMetaObject(strClassName.toStdString().data(), &TypeObject::_staticMetaObject);
}

void QQuickTypeObject::startElapseTimer()
{
    _timerElapsed.start();
}

qint64 QQuickTypeObject::elapsed() const
{
    return _timerElapsed.elapsed();
}

class Thread : public QThread
{
public:
    enum TaskType {
        enUnknown,
        enRDF_Split,
        enRDF_MOC,
        enRDF_codeCPP
    };

    struct TD_DATA {
        double *progress;
        QString path_rdf;
        QString out_dir;
        QString path_db;

        TaskType nTaskType;

        void init(){
            progress = 0;
            nTaskType = enUnknown;
            path_rdf = out_dir = path_db = "";
        }
    }*m_daa;

    LogMe *_logger;

public:
    Thread(TD_DATA *_daa) : m_daa(_daa), _logger(new LogMe(this)) {}
    ~Thread() {
        _logger->info("Thread quited ! ");

        DELETE_POINTER(m_daa);
        DELETE_POINTER(_logger);
    }

protected:
    void run()
    {
        /* 线程的相关代码 */
        if (0 == m_daa || m_daa->nTaskType < enRDF_Split || m_daa->nTaskType > enRDF_codeCPP)
            return;

        TD_DATA &daa = *m_daa;
        RDFParser parse;

        switch(daa.nTaskType){
        case enRDF_Split:
            _logger->info("starting a new Task : Split RDF ");
            if (parse.splitRDF(daa.path_rdf.toStdString().data(), daa.out_dir.toStdString().data()
                               , *(daa.progress)))
            {
                _logger->info("Operation -> Split RDF : Complete !");
            }else{
                _logger->info("Operation Failed !");
            }
            break;
        case enRDF_MOC:
            _logger->info("starting a new Task : Moc RDF Meta Datum ");
            if (parse.mocMetaDatum(daa.path_rdf.toStdString().data(), daa.path_db.toStdString().data()
                                   , daa.out_dir.toStdString().data()
                                   , *(daa.progress))){
                _logger->info("Operation -> Moc Meta Datum : Complete !");
            }else{
                _logger->info("Operation Failed !");
            }
            break;
        case enRDF_codeCPP:
            _logger->info("starting a new Task : Code RDF C++ Source ");
            if (parse.code2CppSource(daa.path_rdf.toStdString().data(),
                                     QString("").toStdString().data(),
                                     daa.out_dir.toStdString().data(), *(daa.progress))){
                _logger->info("Operation -> Code Cpp Source : Complete !");
            }else{
                _logger->info("Operation Failed !");
            }
            break;
        default: break;
        }
    }
};

bool QQuickTypeObject::runRdfTask(int type, QString path_rdf, QString path_db, QString out_dir)
{
    Thread::TD_DATA *td = new Thread::TD_DATA;
    td->init();
    td->nTaskType = (Thread::TaskType)type;
    td->path_rdf = path_rdf;
    td->path_db = path_db;
    td->out_dir = out_dir;
    td->progress = &m_progress;

    Thread *thread = new Thread(td);
    connect(thread, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

    return true;
}

bool QQuickTypeObject::isRdfExist(QString path_rdf)
{
    return QFileInfo::exists(path_rdf);
}

QString QQuickTypeObject::appDirPath()
{
    return QCoreApplication::applicationDirPath();
}

QString QQuickTypeObject::metalib_dbPath()
{
    QString path = appDirPath();

    return path + "/../data/cim/metalib.db";
}
