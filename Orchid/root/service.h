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


#ifndef _ORCHID_SERVICE_H_
#define _ORCHID_SERVICE_H_

#include <QObject>

namespace Orchid {

namespace Resource {
class Handle;
}

class ServicePrivate;

class Service : public QObject {
	Q_OBJECT
public:
	Service();
public:
	void setRoot(const Resource::Handle& root);
	Resource::Handle root() const;
protected:
	Service(ServicePrivate* dd);
private:
	Q_DECLARE_PRIVATE(Service)
protected:
	ServicePrivate* d_ptr;

};

}

#endif
