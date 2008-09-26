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
	ResourceModel(Resource::Handle root, QObject* parent = 0);
	~ResourceModel();
public:
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	
	QString path(const QModelIndex &index) const;
	QString name(const QModelIndex &index) const;
	QString info(const QModelIndex &index) const;
	Resource::Ownership ownership(const QModelIndex &index) const;
	
	Resource::Handle resource(const QModelIndex &index) const;
public slots:
	void update(const QModelIndex &index);
	void update();
protected:
	ResourceModelPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ResourceModel)
	Q_DISABLE_COPY(ResourceModel)
};

}

#endif
