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


#include "resource.h" 
#include "request.h"
#include "resourcekeep.h"

#include "resourcebase_p.h"

#include <QTextStream>
#include <QMutex>

#include <QtDebug>


namespace Orchid {

static QMutex interfaceRegMutex;
static QHash<QByteArray,int> interfaceList;

int regInterface(const char *name) {
	QByteArray key(name);
	QMutexLocker locker(&interfaceRegMutex);
	static int nextId = 1;

	int id = interfaceList.value(key);
	if(!id) {
		id = nextId++;
		interfaceList.insert(key, id);
	}

	return id;
}

class ContainerResourcePrivate : public Resource::BasePrivate {
	Q_DECLARE_PUBLIC(ContainerResource)
public:
	ContainerResourcePrivate(ContainerResource* resource);
private:
	QStringList m_childs;
};

ContainerResourcePrivate::ContainerResourcePrivate(ContainerResource* resource) 
	: BasePrivate(resource)
{ }

ContainerResource::ContainerResource() : Base(new ContainerResourcePrivate(this)) {
}

ContainerResource::~ContainerResource() {
}


bool ContainerResource::addResource(const QString& name, Resource::Base* res, Resource::Ownership ownership) {
	Q_D(ContainerResource);
	Resource::Handle handle = keep()->acquireHandle(name);
	if(!handle.isEmpty()) return false;
	handle.init(res, ownership, Resource::KeepPersistant);
	d->m_childs << name;
	return true;
}

bool ContainerResource::remove(const QString &name) {
	return keep()->reset(name);
}

bool ContainerResource::removeAll() {
	Q_D(ContainerResource);
	keep()->resetAll();
	d->m_childs.clear();
	return true;
}

QStringList ContainerResource::childs() const {
	Q_D(const ContainerResource);
	return d->m_childs;
}

Orchid::Resource::Handle ContainerResource::child(const QString& name) {
	Q_D(ContainerResource);
	Resource::Handle handle = keep()->acquireHandle(name);
	if(handle.isEmpty())
		return Resource::Handle();
	return handle;
}

}
