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


#include "styleattributes.h" 

#include <QString>

namespace Orchid {

/**
 * \class StyleAttributes
 *
 * \note This class is currently unmaintained
 */

StyleAttributes::StyleAttributes() {
	d = new StyleAttributesData;
}

StyleAttributes::StyleAttributes(const QString& prefix) {
	d = new StyleAttributesData;
	d->prefix = prefix;
}

QString StyleAttributes::classname(const QString& classname) {
	if(d->prefix.isNull()) return QString();
	return d->prefix+classname;
}

}
