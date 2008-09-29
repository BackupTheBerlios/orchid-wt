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

/**
 * \file resource.h
 * \brief Provides all you need for implmenting standard resources.
 */

/**
 * \def ORCHID_DECLARE_INTERFACE(type)
 *
 * Makes interface \a type a valid Orchid interface that can be
 * cast to by Resource::cast<T>()
 */

namespace Resource {

/**
 * \class IDirectory
 *
 * \brief Resource::IDirectory is an interface for access to
 * children
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
 * \brief Resource::IContainer is an interface for managing child
 * resources.
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
 * \brief Resource::IRedirecting is an interface for redirecting
 * requests to for resources to other resources.
 *
 * When a Location resolves a path and this path contains a resource
 * with the IRedirecting interface, it calls locate() with the rest
 * of the path and returns the result.
 *
 * \deprecated This interface might be removed in later versions.
 */

/**
 * \fn Handle IRedirecting::locate(const QUrl &url) = 0
 *
 * Returns the handle for the resource specified by \a url.
 * This locate call can redirect to anywhere...
 *
 * \deprecated This function might be removed in later versions.
 */

/**
 * \class IQueryable
 *
 * \brief Resource::IQueryable is an interface for resources that
 * can be requested over the web.
 *
 * \sa Request::Base
 */

/**
 * \fn void IQueryable::query(Request* request) = 0
 *
 * Queries the resource with \a request. You can respond to this
 * request by opening \a request and writing to it.
 */

/**
 * \class IConfigurable
 *
 * \brief Resource::IConfigurable is an interface for configuring
 * resources.
 *
 * Resources that implement IConfigurable can be configured in
 * the Orchid Browser tool and their configuration can be stored
 * in an service setup file.
 */

/**
 * \fn QList<Option> IConfigurable::optionList() const = 0
 *
 * Returns a list of supported options as pairs of the name as
 * QString and the qMetaTypeId of the supported type as integer.
 */

/**
 * \fn QVariant IConfigurable::option(const QString &option) const = 0
 *
 * Returns the current value of \a option or QVariant() if
 * \a option does not exist or has no value.
 */

/**
 * \fn bool IConfigurable:setOption(const QString &option, const QVariant &value) = 0
 *
 * Sets the option \a option to \a value and returns true or false
 * when \a option is not a supported option or \a value is
 * currently not allowed for \a option.
 */

/**
 * \class IAdvancedConfigurable
 * 
 * \brief Resource::IAdvancedConfigurable is an extension to
 * IConfigurable that provides more information about options.
 *
 * IAdvancedConfigurable provides can provide advanced information
 * about options, like possible values or a short description.
 *
 * Currently no list of possible properties is defined.
 * This will change in future versions.
 */

/**
 * \fn QVariant IAdvancedConfigurable::optionProperty(const QString &option, const QString &property) const = 0
 *
 * Returns the property \a property for \a option if one is provided
 * or QVariant() otherwise.
 */

/**
 * \fn T cast(Base *res)
 *
 * Returns \a res cast to the interface T. This uses the Qt object
 * metatype system for classes derived from Resource::Object or
 * Resource::Base::interfaceCast() otherwise.
 *
 * T needs to be declared as an resource interface by
 * ORCHID_DECLARE_INTERFACE(T, name).
 */

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

/**
 * \class ContainerResource
 *
 * \brief ContainerResource provides an standard implementation of
 * containers.
 *
 * Use this class everywhere where you need to add container
 * which you can add resources to.
 */

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

/**
 * Cosntructs a new container.
 */
ContainerResource::ContainerResource() : Base(new ContainerResourcePrivate(this)) {
}

/**
 * Desturcts the container.
 */
ContainerResource::~ContainerResource() {
}

/**
 * Adds \a res to the container as \a name with the ownership
 * \a ownership. Returns true if no resource with this name
 * is allready present or false otherwise.
 *
 * \sa IContainer
 */
bool ContainerResource::addResource(const QString& name, Resource::Base* res, Resource::Ownership ownership) {
	Q_D(ContainerResource);
	Resource::Handle handle = keep()->acquireHandle(name);
	if(!handle.isEmpty()) return false;
	handle.init(res, ownership, Resource::KeepPersistant);
	d->m_childs << name;
	return true;
}

/**
 * Removes child \a name from the container and returns true on
 * success or false if the container had no such child.
 *
 * \sa IContainer
 */
bool ContainerResource::remove(const QString &name) {
	return keep()->reset(name);
}

/**
 * Removes all childs and returns true.
 *
 * \sa IContainer
 */
bool ContainerResource::removeAll() {
	Q_D(ContainerResource);
	keep()->resetAll();
	d->m_childs.clear();
	return true;
}

/**
 * Returns a list of childs.
 *
 * \sa IDirectory
 */
QStringList ContainerResource::childs() const {
	Q_D(const ContainerResource);
	return d->m_childs;
}

/**
 * Returns child \a name or Handle() if \a name is no child
 * of the container.
 *
 * \sa IDirectory
 */
Orchid::Resource::Handle ContainerResource::child(const QString& name) {
	Q_D(ContainerResource);
	Resource::Handle handle = keep()->acquireHandle(name);
	if(handle.isEmpty())
		return Resource::Handle();
	return handle;
}

/**
 * \internal
 */
void *ContainerResource::interfaceCast(const char *name) {
	ORCHID_PROVIDE_CAST(name, Resource::IDirectory*)
	ORCHID_PROVIDE_CAST(name, Resource::IContainer*)
	return 0;
}

}
