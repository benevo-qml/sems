#ifndef RDFPROPERTYMODEL_H
#define RDFPROPERTYMODEL_H

#include <QStandardItemModel>

class RdfPropertyModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit RdfPropertyModel(QObject *parent = 0);
    virtual ~RdfPropertyModel() {}

    enum RdfPropertyModel_Roles
    {
        Role_RdfName = Qt::DisplayRole,
        Role_RdfValue = Qt::UserRole,
    };

    QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE bool retrieveNewModel(QString strClassName);

private:
    void addEntry( const QString& name, const QString& type, const QString& description);
    QStandardItem* getBranch( const QString& branchName );

private:
    QHash<int, QByteArray> m_roleNames;

};

#endif // RDFPROPERTYMODEL_H
