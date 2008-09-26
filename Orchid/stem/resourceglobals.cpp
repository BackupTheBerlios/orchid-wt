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


// NOTE This File contains only documentation for the namespaces of Orchid
// and will not be compiled!

namespace Orchid {

namespace Resource {

/**
 * \enum Ownership
 *
 * \brief Enumerates the different ownership modes.
 *
 * \note This enum is likely to be replaced.
 */

/**
 * \var Ownership OwnedPrivate
 *
 * Thought to be used for resource that are completely managed
 * by another resource and don't have their own configuration.
 */

/**
 * \var Ownership OwnedInternal
 *
 * Thought to be used for resources that are generated and managed
 * by another resource but needs its own configuration entry.
 */

/**
 * \var Ownership OwnedExternal
 *
 * Thought to be used for resources that are build based on the
 * configuration file and don't have other resources, that manage
 * them.
 */

}

}