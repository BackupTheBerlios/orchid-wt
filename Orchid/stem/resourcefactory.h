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


#ifndef _ORCHID_RESOURCEFACTORY_H_
#define _ORCHID_RESOURCEFACTORY_H_

#include "extensionmanager.h"

#include <stem/resourcebase.h>

class QString;

namespace Orchid {

namespace Resource {
class Base;
}

class ResourceFactoryHelperBase : public FactoryHelper {
public:
	virtual Resource::Base *create() = 0;
};

template <class T>
class ResourceFactoryHelper : public ResourceFactoryHelperBase {
public:
	ResourceFactoryHelper() {
		// compiletime check for ORCHID_RESOURCE macro in T
		T *p = static_cast<T*>(0);
		p->checkForResourceMacro(p);
	}
public:
	QString key() const {
		return T::staticTypeName();
	}
	Resource::Base *create() {
		return new T();
	}
};

class ResourceFactory {
public:
	static QStringList keys();
	static Resource::Base *create(const QString &key);
};

}

#endif
