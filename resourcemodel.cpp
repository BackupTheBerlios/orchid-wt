#include "resourcemodel.h"

#include "resourcemodel.moc"

// TODO clean up

#include <QtDebug>

namespace Orchid {

bool ModelResource::addResource(const QString& name, Resource::Resource* res) {
	Orchid::Resource::Handle handle = m_keep.getHandle(name);
	handle.init(res, Orchid::Resource::KeepPersistant);
	m_childs.insert(name, handle);
	return true;
}

QStringList ModelResource::childs() const {
	return m_childs.keys();
}

Orchid::Resource::Handle ModelResource::child(const QString& name) const {
	return m_childs.value(name);
}

class ResourceModelPrivate {
public:
	class Node {
	public:
		Node() {
			parent = 0;
			populated = false;
		}
		Node(Node* parent, Resource::Handle handle) {
			this->parent = parent;
			this->handle = handle;
			populated = false;
		}
	public:
		Node* parent;
		Resource::Handle handle;
		QVector<Node> childs;
		bool populated;
	};
public:
	ResourceModelPrivate(ResourceModel* model, Resource::Handle res);
	Node* node(const QModelIndex& index) const;
	void populate(Node* node) const;
public:
	Node* root;
	
protected:
    ResourceModel* q_ptr;
private:
	Q_DECLARE_PUBLIC(ResourceModel)
};

ResourceModelPrivate::ResourceModelPrivate(ResourceModel* model, Resource::Handle res) : q_ptr(model) {
	root = new Node(0, res);
}

ResourceModelPrivate::Node* ResourceModelPrivate::node(const QModelIndex& index) const {
	return static_cast<Node*>(index.internalPointer());
}

void ResourceModelPrivate::populate(Node* node) const {
	if(node->populated) return;
	
	Orchid::Resource::IDirectory* dir = dynamic_cast<Orchid::Resource::IDirectory*>(node->handle.resource());
	
	if(!dir) { node->populated = true; return; }
	
	QStringList list = dir->childs();
	QVector<ResourceModelPrivate::Node> childs(list.count());
	
	for(int i = list.count()-1; i >= 0; --i) {
		Node &child = childs[i];
		child.parent = node;
		child.handle = dir->child(list[i]);
	}
	node->childs = childs;
	node->populated = true;
}


ResourceModel::ResourceModel(Resource::Resource* resource, QObject* parent) : QAbstractItemModel(parent) {
	Resource::Handle handle;
	handle.init(resource);
	d_ptr = new ResourceModelPrivate(this, handle);
}

ResourceModel::~ResourceModel() {
	delete d_ptr;
}

QModelIndex ResourceModel::index(int row, int column, const QModelIndex &parent) const {
	Q_D(const ResourceModel);
	
	ResourceModelPrivate::Node* p = parent.isValid() ? d->node(parent) : d->root;

	if(!p) return QModelIndex();

	if(!p->populated)
		d->populate(p);
	
	if(row < 0 || row >= p->childs.count());

	return createIndex(row, column, &p->childs[row]);
}

QModelIndex ResourceModel::parent(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	
	ResourceModelPrivate::Node* node = d->node(index);
	ResourceModelPrivate::Node* parent = node ? node->parent : 0;
	ResourceModelPrivate::Node* pp = parent ? parent->parent : 0;
	
	if(!pp) return QModelIndex();
	
	int row = parent - &pp->childs[0];
	
	return createIndex(row, 0, parent);
}

int ResourceModel::rowCount(const QModelIndex &parent) const {
	Q_D(const ResourceModel);

	ResourceModelPrivate::Node* node = parent.isValid() ? d->node(parent) : d->root;
	
	if(!node) return 0;
	
	if(!node->populated)
		d->populate(node);

	return node->childs.count();
}

int ResourceModel::columnCount(const QModelIndex &parent) const {
	return 2;
}

QVariant ResourceModel::data(const QModelIndex &index, int role) const {
	Q_D(const ResourceModel);
	if(role != Qt::DisplayRole) return QVariant();
	
	ResourceModelPrivate::Node *node = d->node(index);
	
	if(index.column() == 0) {
		return node->handle.name();
	} else {
		Resource::Resource* resource = node->handle.resource();
		if(dynamic_cast<Resource::IDirectory*>(resource))
			return "Directory";
		if(dynamic_cast<Resource::IQueryable*>(resource))
			return "Queryable";
		return "Resource";
	}
}


}
