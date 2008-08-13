#ifndef _ORCHID_RESOURCEMODEL_H_
#define _ORCHID_RESOURCEMODEL_H_

#include <QAbstractItemModel>
#include <QHash>

#include "resource.h"
#include "resourcekeep.h"

namespace Orchid {

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
	
	QString path(const QModelIndex &index) const;
	
	Resource::Handle resource(const QModelIndex &index) const;
protected:
	ResourceModelPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ResourceModel)
	Q_DISABLE_COPY(ResourceModel)
};

}

#endif
