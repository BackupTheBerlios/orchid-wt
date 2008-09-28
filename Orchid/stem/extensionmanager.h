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


#ifndef _ORCHID_EXTIONSIONMANAGER_H_
#define _ORCHID_EXTIONSIONMANAGER_H_

#include "globals.h"

class QString;
class QStringList;

namespace Orchid {

class ORCHID_STEM_EXPORT FactoryHelper {
public:
	virtual ~FactoryHelper() {}
public:
	virtual QString key() const = 0;
};

class ORCHID_STEM_EXPORT ExtensionManager {
public:
	static bool loadExtension(const QString &path);
	static QStringList keys(const QString &category);
	static FactoryHelper *helper(const QString &category, const QString &key);
};

}

#endif
