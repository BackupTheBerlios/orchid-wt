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


#include "service.h"
#include "service_p.h"

namespace Orchid {

/**
 * \class Service
 * 
 * \brief Service provides the base class for resource based services.
 *
 * \sa Resource::Object, HttpService
 */

/**
 * Constructs a new service
 */
Service::Service() : d_ptr(new ServicePrivate(this)) {
}

/**
 * \internal
 */
Service::Service(ServicePrivate* dd) : d_ptr(dd) {
}

/**
 * Sets \a root of the resource-tree used by the service.
 *
 * \sa root()
 */
void Service::setRoot(const Resource::Handle& root) {
	Q_D(Service);
	d->root = root;
}

/**
 * Resturns the root resource of the service.
 *
 * \sa setRoot()
 */
Resource::Handle Service::root() const {
	Q_D(const Service);
	return d->root;
}

}
