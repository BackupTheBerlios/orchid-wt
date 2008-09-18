#include "resourcemodel.h"

#include "resourcemodel.moc"

#include "location.h"

// TODO clean up

#include <QVector>

namespace Orchid {

class ResourceModelPrivate {
public:
	class Node {
	public:
		Node() {
			parent = 0;
			populated = false;
			hasInfos = false;
		}
		Node(Node* parent, Resource::Location location) {
			this->parent = parent;
			this->location = location;
			populated = false;
			hasInfos = false;
		}
	public:
		Node* parent;
		QVector<Node> childs;
		Resource::Location location;
		QString name;
		QString info;
		Resource::Ownership ownership;
		bool populated : 1;
		bool hasInfos : 1;
	};
public:
	ResourceModelPrivate(ResourceModel* model, Resource::Handle res);
	Node* node(const QModelIndex& index) const;
	void populate(Node* node) const;
	void loadInfos(Node* node) const;
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

void ResourceModelPrivate::loadInfos(Node* node) const {
	Resource::Handle handle = node->location.resource();
	Resource::Base *res = handle.resource();
	
	node->name = handle.name();
	node->ownership = handle.ownership();
	if(Resource::cast<Resource::IDirectory*>(res)) {
		node->info = "Directory";
	} else if(Resource::cast<Resource::IQueryable*>(res)) {
		node->info = "Queryable";
	} else {
		node->info = "Resource";
	}
	node->hasInfos = true;
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
	switch(role) {
		case Qt::DisplayRole:
			if(index.column() == 0) {
				return name(index);
			} else if(index.column() == 1) {
				return info(index);
			}
			break;
		case Qt::ToolTipRole:
			return path(index);
		case Qt::ForegroundRole: {
			switch(ownership(index)) {
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
	if(!node->hasInfos) d->loadInfos(node);
	return '/' + node->location.path();
}

QString ResourceModel::name(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return QString();
	if(!node->hasInfos) d->loadInfos(node);
	return node->name;
}

QString ResourceModel::info(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return QString();
	if(!node->hasInfos) d->loadInfos(node);
	return node->info;
}

Resource::Ownership ResourceModel::ownership(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return Resource::OwnedPrivate;
	if(!node->hasInfos) d->loadInfos(node);
	return node->ownership;
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
