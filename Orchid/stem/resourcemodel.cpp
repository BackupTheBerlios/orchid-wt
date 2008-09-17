#include "resourcemodel.h"

#include "resourcemodel.moc"

#include "location.h"

// TODO clean up

#include <QtDebug>

namespace Orchid {

class ResourceModelPrivate {
public:
	class Node {
	public:
		Node() {
			parent = 0;
			populated = false;
		}
		Node(Node* parent, Resource::Location location) {
			this->parent = parent;
			this->location = location;
			populated = false;
		}
	public:
		Node* parent;
		Resource::Location location;
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
	root = new Node(0, Resource::Location(res, ""));
}

ResourceModelPrivate::Node* ResourceModelPrivate::node(const QModelIndex& index) const {
	return static_cast<Node*>(index.internalPointer());
}

void ResourceModelPrivate::populate(Node* node) const {
	if(node->populated) return;
	
	Resource::Handle handle = node->location.resource();
	Resource::IDirectory* dir = dynamic_cast<Resource::IDirectory*>(handle.resource());
	
	if(!dir) { node->populated = true; return; }
	
	QStringList list = dir->childs();
	if(list.count() != 0) {
		QVector<ResourceModelPrivate::Node> childs(list.count());	
		QStringList::iterator it;
		Node *child = &childs[0];
		for(it = list.begin(); it != list.end(); ++it, ++child) {
			child->parent = node;
			child->location = node->location.relative(*it);
		}
		node->childs = childs;
	}
	node->populated = true;
}


ResourceModel::ResourceModel(Resource::Handle root, QObject* parent) : QAbstractItemModel(parent) {
	d_ptr = new ResourceModelPrivate(this, root);
}

ResourceModel::~ResourceModel() {
	delete d_ptr;
}

QModelIndex ResourceModel::index(int row, int column, const QModelIndex &parent) const {
	Q_D(const ResourceModel);

	if(parent.isValid() && parent.column() != 0) return QModelIndex();
	
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

	if(parent.isValid() && parent.column() != 0) return 0;

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
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return QVariant();
	
	switch(role) {
		case Qt::DisplayRole:
			if(index.column() == 0) {
				return node->location.name();
			} else if(index.column() == 1) {
				Resource::Handle handle = node->location.resource();
				Resource::Base* resource = handle.resource();
				if(dynamic_cast<Resource::IDirectory*>(resource))
					return "Directory";
				if(dynamic_cast<Resource::IQueryable*>(resource))
					return "Queryable";
				return "Resource";
			}
			break;
		case Qt::ToolTipRole:
			return path(index);
		case Qt::ForegroundRole: {
			Resource::Handle handle = node->location.resource();
			switch(handle.ownership()) {
				case Resource::OwnedPrivate:
					return Qt::lightGray;
				case Resource::OwnedInternal:
					return Qt::darkGray;
				case Resource::OwnedExternal:
					return Qt::black;
			}
		}
	}
	return QVariant();
}

QString ResourceModel::path(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return QString();
	return '/' + node->location.path();
}

Resource::Handle ResourceModel::resource(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return Resource::Handle();
	
	return node->location.resource();
}

void ResourceModel::update() {
}

}
