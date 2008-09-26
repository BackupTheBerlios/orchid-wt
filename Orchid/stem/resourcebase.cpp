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


#include "resourcebase.h"
#include "resourcebase_p.h"

namespace Orchid {

namespace Resource {

/**
 * \class Base
 *
 * \brief Resource::Base is the base class for resources.
 *
 * All resources should be derived from Resource::Base
 *
 * The resource base provides the keep for temporarily or permanently 
 * storing child resources. If you want to provide child resources
 * you have to implement Resource::IDirectory. For other functionality
 * see the other interfaces.
 *
 * \sa Resource::Keep, Resource::IDirectory, Resource::IContainer,
 * Resource::IQueryable, Resource::IDynamic, Resource::IConfigurable,
 * Resource::IAdvancedConfigurable
 */

BasePrivate::BasePrivate(Base *base) {
	q_ptr = base;
}

/**
 * Constructs a resource base.
 */
Base::Base() {
	d_ptr = new BasePrivate(this);
}

/**
 * \internal
 */
Base::Base(BasePrivate *d) {
	d_ptr = d;
}

/**
 * Desturcts the resource.
 */
Base::~Base() {
	delete d_ptr;
}

/**
 * Returns the keep of a resource.
 *
 * \sa Resource::Keep
 */
Keep *Base::keep() {
	Q_D(Base);
	return &d->keep;
}

}

}
