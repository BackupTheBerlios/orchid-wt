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


#ifndef _STYLEATTRIBUTES_H_
#define _STYLEATTRIBUTES_H_

#include <QSharedData>
#include <QString>

// TODO rename to StyleBinding

namespace Orchid {

class StyleAttributes;

class StyleAttributesData : public QSharedData {
	QString prefix;
	friend class StyleAttributes;
};

class StyleAttributes {
public:
	StyleAttributes();
	StyleAttributes(const QString& prefix);
public:
	QString classname(const QString& name);
private:
	QSharedDataPointer<StyleAttributesData> d;
};

}

#endif 
