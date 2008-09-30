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


#ifndef _ORCHID_XMLMODELRESOURCE_H_
#define _ORCHID_XMLMODELRESOURCE_H_

#include "modelresource.h"

namespace Orchid {

class XmlModelResourcePrivate;
class XmlModelResource : public ModelResource, public Resource::IQueryable {
	ORCHID_RESOURCE("XmlModel")
	Q_OBJECT
	Q_INTERFACES(Orchid::Resource::IQueryable)
public:
	XmlModelResource(QAbstractItemModel* model = 0);
	~XmlModelResource();
public:
	void query(Orchid::Request* request);
	void query(Orchid::Request* request, const QModelIndex& index);
private:
	Q_DECLARE_PRIVATE(XmlModelResource)
};

}

#endif
