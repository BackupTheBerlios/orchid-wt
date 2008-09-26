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


#ifndef _ORCHID_RESOURCELOCATION_H_
#define _ORCHID_RESOURCELOCATION_H_

#include <QtCore/QSharedData>

class QString;

namespace Orchid {

namespace Resource {

class Handle;

class LocationPrivate;
class Location {
public:
	Location();
	Location(Handle root, const QString& path);
	Location(const Location& other, const QString& rel);
	Location(const Location& other);
	~Location();
public:
	bool isNull() const;
	Handle root() const;
	QString path() const;
	QString name() const;
	Handle resolve() const;
	Location relative(const QString& rel) const;
	Location& operator=(const Location &other);
private:
	QSharedDataPointer<LocationPrivate> d;
};

class LocationLookupPrivate;
class LocationLookup {
public:
	LocationLookup();
	LocationLookup(const Handle& root);
	~LocationLookup();
public:
	Handle root() const;
	void setRoot(const Handle& root);
	Handle addLocation(const Location& location);
	Location resolve(const Location& location) const;
protected:
	LocationLookupPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(LocationLookup)
};

}

}

#endif
