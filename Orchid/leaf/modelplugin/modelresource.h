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


#ifndef _ORCHID_MODELRESOURCE_H_
#define _ORCHID_MODELRESOURCE_H_

#include <stem/resourceobject.h>

class QAbstractItemModel;
class QModelIndex;

namespace Orchid {

class ModelResourcePrivate;
class ModelResource :
	public Resource::Object,
	public Resource::IConfigurable
{
	ORCHID_RESOURCE("Model")
	Q_OBJECT
	Q_INTERFACES(Orchid::Resource::IConfigurable)
public:
	ModelResource(QAbstractItemModel* model = 0);
	~ModelResource();
public:
	QAbstractItemModel* model() const;
	void setModel(QAbstractItemModel* model);
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString&);
public:
	virtual void query(Orchid::Request* request, const QModelIndex& index);
	virtual QString name(const QModelIndex& index) const;
	virtual QModelIndex index(const QString& name, const QModelIndex& parent) const;
	virtual QStringList listChilds(const QModelIndex& index) const;
	virtual QList<Option> optionList() const;
	virtual QVariant option(const QString&) const;
	virtual bool setOption(const QString&, const QVariant&);
protected:
	ModelResource(ModelResourcePrivate* d, QAbstractItemModel* model = 0);
	ModelResourcePrivate *d_ptr;
private:
	Q_DECLARE_PRIVATE(ModelResource)
};

};

#endif
