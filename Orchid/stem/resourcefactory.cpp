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


#include "resourcefactory.h"

#include "resource.h"

namespace Orchid {
	
QStringList ResourceFactory::keys() {
	QStringList list;
	list << ContainerResource::staticTypeName();
	list << ExtensionManager::keys("resource");
	return list;
}

Resource::Base *ResourceFactory::create(const QString &key) {
	Resource::Base *res = 0;
	if(key == ContainerResource::staticTypeName()) {
		res = new ContainerResource();
	}
	if(!res) {
		FactoryHelper* helper = ExtensionManager::helper("resource", key);
		ResourceFactoryHelperBase *resHelper = dynamic_cast<ResourceFactoryHelperBase*>(helper);
		if(helper)
			res = resHelper->create();
	}
	return res;
}

}
