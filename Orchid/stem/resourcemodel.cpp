/*
 * This file is part of the Orchid libraries
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "resourcemodel.h"

#include "location.h"

// TODO clean up

#include <QtCore/QVector>

namespace Orchid {

/**
 * \class ResourceModel
 *
 * \brief ResourceModel provides a model representating the resource
 * tree.
 */

class ResourceModelPrivate {
public:
	class Node {
	public:
		Node() {
			row = 0;
			parent = 0;
			populated = false;
			hasInfos = false;
		}
		Node(Node* parent, Resource::Location location) {
			row = 0;
			this->parent = parent;
			this->location = location;
			populated = false;
			hasInfos = false;
		}
		~Node() {
			QVector<Node*>::iterator it;
			for(it = childs.begin(); it != childs.end(); ++it) {
				delete *it;
			}
		}
	public:
		Node* parent;
		QVector<Node*> childs;
		Resource::Location location;
		QString name;
		QString info;
		Resource::Ownership ownership;
		int row;
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
	if(!index.isValid()) return 0;
	return static_cast<Node*>(index.internalPointer());
}

void ResourceModelPrivate::populate(Node* node) const {
	if(node->populated) return;
	
	Resource::Handle handle = node->location.resolve();
	
	Resource::IDirectory* dir = Resource::cast<Resource::IDirectory*>(handle.resource());
	
	if(!dir) { node->populated = true; return; }
	
	QStringList list = dir->childs();
	if(list.count() != 0) {
		int row = 0;
		QStringList::iterator it;
		for(it = list.begin(); it != list.end(); ++it, ++row) {
			Node *child = new Node();
			child->parent = node;
			child->location = node->location.relative(*it);
			child->row = row;
			node->childs << child;
		}
	}
	node->populated = true;
}

void ResourceModelPrivate::loadInfos(Node* node) const {
	Resource::Handle handle = node->location.resolve();
	Resource::Base *res = handle.resource();

	Q_ASSERT(res);
	
	if(node == root) {
		node->name = "Root";
		node->ownership = Resource::OwnedExternal;
		node->info = "";
		node->hasInfos = true;
	} else {
		node->name = handle.name();
		node->ownership = handle.ownership();
		node->info = res->typeName();
		node->hasInfos = true;
	}
}

/**
 * Constructs a new resource model with the given \a root
 * and \a parent.
 */
ResourceModel::ResourceModel(Resource::Handle root, QObject* parent) : QAbstractItemModel(parent) {
	d_ptr = new ResourceModelPrivate(this, root);
}

/**
 * Desturcts the model.
 */
ResourceModel::~ResourceModel() {
	delete d_ptr;
}

/**
 * Reimplementation of QAbstractItemModel::index().
 */
QModelIndex ResourceModel::index(int row, int column, const QModelIndex &parent) const {
	Q_D(const ResourceModel);

	if(!parent.isValid()) return createIndex(row, column, d->root);
	if(parent.column() != 0) return QModelIndex();

	ResourceModelPrivate::Node* p = d->node(parent);

	Q_ASSERT(p);

	if(!p->populated)
		d->populate(p);
	
	return createIndex(row, column, p->childs[row]);
}

/**
 * Reimplementation of QAbstractItemModel::parent().
 */
QModelIndex ResourceModel::parent(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	
	ResourceModelPrivate::Node* node = d->node(index);
	ResourceModelPrivate::Node* parent = node ? node->parent : 0;
	
	if(!parent) return QModelIndex();
	
	return createIndex(parent->row, 0, parent);
}

/**
 * Reimplementation of QAbstractItemModel::rowCount()
 */
int ResourceModel::rowCount(const QModelIndex &parent) const {
	Q_D(const ResourceModel);

	if(!parent.isValid()) return 1;
	
	if(parent.column() != 0) return 0;

	ResourceModelPrivate::Node* node = d->node(parent);
	Q_ASSERT(node);
	
	if(!node->populated)
		d->populate(node);

	return node->childs.count();
}

/**
 * Reimplementation of QAbstractItemModel::columnCount()
 */
int ResourceModel::columnCount(const QModelIndex &parent) const {
	return 2;
}

/**
 * Reimplementation of QAbstractItemModel::data()
 */
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

/**
 * Returns the path of the resource represented by \a index.
 */
QString ResourceModel::path(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return QString();
	if(!node->hasInfos) d->loadInfos(node);
	return '/' + node->location.path();
}

/**
 * Returns the name of the resource represented by \a index.
 */
QString ResourceModel::name(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return QString();
	if(!node->hasInfos) d->loadInfos(node);
	return node->name;
}

/**
 * Returns an information string about the resource represented
 * by \a index.
 */
QString ResourceModel::info(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return QString();
	if(!node->hasInfos) d->loadInfos(node);
	return node->info;
}

/**
 * Returns the ownership flag of the resource represented by \a index.
 */
Resource::Ownership ResourceModel::ownership(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return Resource::OwnedPrivate;
	if(!node->hasInfos) d->loadInfos(node);
	return node->ownership;
}

/**
 * Returns the resource represented by \a index.
 */
Resource::Handle ResourceModel::resource(const QModelIndex &index) const {
	Q_D(const ResourceModel);
	
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return Resource::Handle();
	
	return node->location.resolve();
}

/**
 * Updates the item at \a index and its childs.
 */
void ResourceModel::update(const QModelIndex &index) {
	Q_D(const ResourceModel);
	
	ResourceModelPrivate::Node *node = d->node(index);
	if(!node) return;
		// ok reached a point where src and dest differ
	
	d->loadInfos(node);

	Resource::Handle handle = node->location.resolve();
	Resource::Base *res = handle.resource();
	QStringList childs;
	Resource::IDirectory *dir = Resource::cast<Resource::IDirectory*>(res);
	if(dir) {
		childs = dir->childs();
	}
	
	int destRest = 0;
	int srcRest = 0;
	while(destRest < node->childs.count() || srcRest < childs.count()) {
		while(destRest < node->childs.count() && srcRest < childs.count()) {
			if(node->childs[destRest]->name != childs[srcRest])
				break;
			
			update(createIndex(destRest, 0, node->childs[destRest]));
			
			node->childs[destRest]->row = destRest;
			++destRest; ++srcRest;
		}
		// ok reached a point where src and dest differ
		// search for good new combination
		int destTest = destRest, srcTest = srcRest;
		for(destTest = destRest; destTest < node->childs.count(); ++destTest) {
			// TODO optimize this
			for(srcTest = srcRest; srcTest < childs.count(); ++srcTest) {
				if(node->childs[destTest]->name == childs[srcTest])
					break;
			}
			if(srcTest < childs.count())
				break;
		}
		if(destTest == node->childs.count()) {
			// ok we reached the end, insert the full rest
			srcTest = childs.count();
		}
		
		// we searched to the next pair with the same name or the end
		if(destRest != destTest && destRest < node->childs.count()) {
			beginRemoveRows(index, destRest, destTest-1);
			for(int i = destRest; i < destTest; ++i) {
				delete node->childs[i];
			}
			node->childs.remove(destRest, destTest-destRest);
			endRemoveRows();
		}
		int newCount = srcTest-srcRest;
		if(newCount != 0 && srcRest < childs.count()) {
			beginInsertRows(index, destRest, destRest+newCount-1);
			node->childs.insert(destRest, newCount, 0);
			for(int i = 0; i < newCount; ++i) {
				ResourceModelPrivate::Node *child = new ResourceModelPrivate::Node();
				child->parent = node;
				child->location = node->location.relative(childs[srcRest+i]);
				child->row = destRest+i;
				node->childs[destRest+i] = child;
			}
			destRest += newCount;
			srcRest = srcTest;
			endInsertRows();
		}
	}
}

/**
 * Updates the whole tree.
 */
void ResourceModel::update() {
	Q_D(ResourceModel);
	update(createIndex(0, 0, d->root));
}

}
