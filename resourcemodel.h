#ifndef _ORCHID_RESOURCEMODEL_H_
#define _ORCHID_RESOURCEMODEL_H_

#include <QAbstractItemModel>
#include <QHash>

#include "resource.h"
#include "resourcekeep.h"

namespace Orchid {

class ModelResource : public Resource::Resource, public Resource::IDirectory {
public:
	bool addResource(const QString& name, Resource::Resource* res);
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString& name) const;
public:
	Orchid::Resource::Keep m_keep;
	QHash<QString, Orchid::Resource::Handle> m_childs;
};

class ResourceModelPrivate;
class ResourceModel : public QAbstractItemModel {
	Q_OBJECT
public:
	ResourceModel(Resource::Resource* resource, QObject* parent = 0);
	~ResourceModel();
public:
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	
	Resource::Handle resource(const QModelIndex &index) const;
protected:
	ResourceModelPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ResourceModel)
	Q_DISABLE_COPY(ResourceModel)
};

}

#endif
