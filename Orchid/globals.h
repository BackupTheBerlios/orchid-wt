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


#ifndef _ORCHID_GLOBALS_H_
#define _ORCHID_GLOBALS_H_

#define ORCHID_VERSION_STR "0.1 alpha 2"

#include <QtCore/QtGlobal>

#ifdef ORCHID_BUILD_STEM
#	define ORCHID_STEM_EXPORT Q_DECL_EXPORT
#else
#	define ORCHID_STEM_EXPORT Q_DECL_IMPORT
#endif
#ifdef ORCHID_BUILD_FLOWER
#	define ORCHID_FLOWER_EXPORT Q_DECL_EXPORT
#else
#	define ORCHID_FLOWER_EXPORT Q_DECL_IMPORT
#endif
#ifdef ORCHID_BUILD_ROOT
#	define ORCHID_ROOT_EXPORT Q_DECL_EXPORT
#else
#	define ORCHID_ROOT_EXPORT Q_DECL_IMPORT
#endif
#ifdef ORCHID_BUILD_LEAF
#	define ORCHID_LEAF_EXPORT Q_DECL_EXPORT
#else
#	define ORCHID_LEAF_EXPORT Q_DECL_IMPORT
#endif

#endif
