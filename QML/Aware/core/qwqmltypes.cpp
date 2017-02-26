/// QML Type Registration Helper
#include <QtQml>
#include "metalib/corelib/metaobject.h"
#include "core/cim/qquicktypebbject.h"
#include "core/cim/RdfPropertyModel.h"
#include "cim/cim.h"
#include "itunes/core/logme.h"
//#include "quicko3/ApplicationWindow.h"

class QWQmlTypes {

public:
    QWQmlTypes() {
        //qmlRegisterType<ApplicationWindow>("QuickWoo", 1,0, "AppWindow");
        qmlRegisterType<QQuickTypeObject>("QuickWoo", 1,0, "QuickTypeObject");
        qmlRegisterType<RdfPropertyModel>("QuickWoo", 1, 0, "RdfPropertyModel" );
        qmlRegisterType<Cim>("QuickWoo", 1, 0, "CIM" );
    }
};

static QWQmlTypes registerHelper;
