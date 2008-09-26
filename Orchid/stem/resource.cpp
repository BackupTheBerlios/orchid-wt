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

namespace Resource {

/**
 * \class IDirectory
 *
 * \brief IDirectory is an interface for access to children
 *
 * If IDirectory was added to an class its child resources are
 * accessible via locations.
 *
 * \sa Location, IContainer
 */

/**
 * \fn QStringList IDirectory::childs() const = 0
 *
 * Returns the list of the names of childs a resource has. Those
 * childs don't have to exist at the time of the call but must
 * be constructable by child().
 *
 * \sa child()
 */

/**
 * \fn Handle IDirectory::child(const QString &name) = 0
 *
 * Returns the child \a name of a resource. This child can be created
 * on request and don't have to stay permanent. See Resource::Keep
 * for more information on handling of child items.
 *
 * \sa childs(), Resource::Keep
 */

/**
 * \class IContainer
 *
 * \brief IContainer is an interface for managing child resources.
 *
 * IContainer is the interface for adding and removing of child
 * resources.
 *
 * \sa IDirectory, Container
 */

/**
 * \fn bool IContainer::addResource(const QString &name, Base *res,
 * Ownership ownership = OwnedExternal) = 0
 *
 * Adds \a res to the resource as \a name with ownership flags
 * \a ownership and returns whether the adding was succesfull.
 * In case of success the resource takes ownership of \a res,
 * otherwise is the caller responsible for deleting \a res.
 * The resource can disallow the adding of resources in which case
 * it will simply return false.
 */

/**
 * \fn bool IContainer::remove(const QString &name) = 0
 *
 * Removes child resource \a name and returns true on success or
 * otherwise false.
 */

/**
 * \fn bool IContainer::removeAll() = 0
 *
 * Removes all child resources and returns true on success or
 * otherwise false.
 */

/**
 * \class IRedirecting
 *
 * \brief IRedirecting is an interface for redirecting requests to
 * for resources to other resources.
 *
 * When a Location resolves a path and this path contains a resource
 * with the IRedirecting interface, it calls locate() with the rest
 * of the path and returns the result.
 *
 * \deprecated This interface might be removed in later versions.
 */

/**
 * \fn Handle locate(const QUrl &url) = 0
 *
 * Returns the handle for the resource specified by \a url.
 * This locate call can redirect to anywhere...
 *
 * \deprecated This function might be removed in later versions.
 */

// TODO add other documentation

}

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
