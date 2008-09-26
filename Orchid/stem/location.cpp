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


#include "location.h"

#include "resourcekeep.h"
#include "resource.h"

#include <QtCore/QHash>

#include <QtCore/QtDebug> // TODO to rem

namespace Orchid {

namespace Resource {

/**
 * \class Location
 *
 * \brief Resource::Location provides the relative location of a
 * resource.
 *
 * With Resource::Location the location of a resource relative to
 * another resource can be stored. The root of a location is
 * allways a handle and the path a string.
 *
 * Only the root will be held in memory while the resource the
 * location points to might be constructed on resolves.
 *
 * \note "." and ".." are neither allowed as resource names nor
 * as part of a path. Only relative() and the relative constructor
 * have are able to handle self and parent links.  All uses
 * elsewhere will result in undefined behavior.
 *
 * \sa LocationLookup, Resource::Base
 */

class LocationPrivate : public QSharedData {
public:
	Handle root;
	QString path;
};

/**
 * Constructs a new location.
 */
Location::Location() {
	d = new LocationPrivate();
}

/**
 * Constructs a new location with \a path relative to \a root.
 * "." and ".." are not allowed in path and will result in
 * undefined behavior.
 */
Location::Location(Handle root, const QString& path) {
	d = new LocationPrivate();
	d->root = root;
	d->path = path;
}

/**
 * Constructs a new location with the root of \a other and \a path
 * relative to \a other.
 *
 * \note Only at the beginning of \a path is an optional sequence of
 * none, one or multiple ".." allowed, where each ".." will be
 * interpreted as a parent-link. Usage of "." or ".." elsewhere
 * in a path is not allowed and results in undefined behavior.
 */
Location::Location(const Location& loc, const QString& path) {
	d = loc.relative(path).d;
}

/**
 * Constructs a copy of \a other.
 */
Location::Location(const Location &other) {
	d = other.d;
}

/**
 * Destructs the location.
 */
Location::~Location() {
}

/**
 * Returns true if this location has no root; otherwise returns false.
 */
bool Location::isNull() const {
	return d->root.isEmpty();
}

/**
 * Returns the root of the location or a invalid Handle if no root
 * was set.
 */
Handle Location::root() const {
	return d->root;
}

/**
 * Returns the path of the location.
 */
QString Location::path() const {
	return d->path;
}

/**
 * Returns the name of the resource the location points to.
 */
QString Location::name() const {
	return d->path.mid(d->path.lastIndexOf('/')+1);
}

/**
 * Resolves the location and returns a handle to the resource
 * the location points to or an invalid handle if an error occured.
 */
Handle Location::resolve() const {
	// NOTE could be optimised by using references on string instead
	// of stringlist
	Base* res = d->root.resource();
	if(d->path.isEmpty()) return d->root;
	
	QStringList path = d->path.split('/');
	Handle handle;
	
	while(res) {
		if(IRedirecting* redir = dynamic_cast<IRedirecting*>(res)) {
			return redir->locate(path.join("/"));
		} else if(IDirectory* dir = dynamic_cast<IDirectory*>(res)) {
			handle = dir->child(path.takeFirst());
		}
		
		if(path.isEmpty()) return handle;
		
		res = handle.resource();
	}
	return Handle();
}

/**
 * Returns the location \a path relative to this one or an invalid
 * location if \a path will point to a parent of the root.
 *
 * \note Only at the beginning of \a path is a sequence of
 * none, one or multiple ".." allowed. Each of that ".." is
 * interpreted as a parent link. Usage of "." or ".." elsewhere
 * in a path is not allowed and results in undefined behavior.
 * 
 * \c Location(root, "foo/bar").relative("../baz") will result
 * in the path "foo/baz".
 */
Location Location::relative(const QString &path) const {
	int p2Skip = 0;
	int p1Length = d->path.length();
	while(QStringRef(&path, p2Skip, 3) == "../") {
		if(p1Length < 0) return Location();
		p2Skip += 3;
		p1Length = d->path.lastIndexOf('/', p1Length)-1;
	}
	if(QStringRef(&path, p2Skip, path.length()-p2Skip) == "..") {
		if(p1Length < 0) return Location();
		p1Length = d->path.lastIndexOf('/', p1Length)-1;
	}
	if(p1Length < 0) return Location();
	if(p1Length == 0) return Location(d->root, path.mid(p2Skip));
	return Location(d->root, d->path.left(p1Length) + '/' + path.mid(p2Skip));
}

/**
 * Assigns \a other to this head and returns a reference to this head.
 */
Location& Location::operator=(const Location &other) {
	d = other.d;
	return *this;
}

/**
 * \class LocationLookup
 *
 * \brief LocationLookup provides an interface for resolving locations
 * with different roots to a common root.
 *
 * Resources don't know their full environment. To get the path
 * of a Location relative to a not root resource, you can use a
 * LocationLookup. If a location was added to the lookup all
 * locations relative to that location can be resolved relative
 * to their common root.
 */

class LocationLookupPrivate {
	Q_DECLARE_PUBLIC(LocationLookup)
public:
	LocationLookupPrivate(LocationLookup* lookup);
protected:
	LocationLookup* q_ptr;
private:
	Handle root;
	QHash<Handle,Location> locations;
};

LocationLookupPrivate::LocationLookupPrivate(LocationLookup* lookup) {
	q_ptr = lookup;
}

/**
 * Constructs a new lookup.
 */
LocationLookup::LocationLookup() {
	d_ptr = new LocationLookupPrivate(this);
}

/**
 * Constructs a new lookup with the common root \a root.
 */
LocationLookup::LocationLookup(const Handle& root) {
	d_ptr = new LocationLookupPrivate(this);
	d_ptr->root = root;
}

/**
 * Destructs the lookup.
 */
LocationLookup::~LocationLookup() {
	delete d_ptr;
}

/**
 * Returns the root of the lookup or an invalid handle if no
 * root was set.
 */
Handle LocationLookup::root() const {
	Q_D(const LocationLookup);
	return d->root;
}

/**
 * Sets the root of the lookup to \a root. This will remove all
 * previously added locations.
 */
void LocationLookup::setRoot(const Handle &root) {
	Q_D(LocationLookup);
	d->root = root;
	d->locations.clear();
}

/**
 * Registers \a location to the lookup. \a location has to be
 * relative to root or a previously added location. Returns the
 * handle of \a location or an invalid handle if an error occured.
 */
Handle LocationLookup::addLocation(const Location& location) {
	Q_D(LocationLookup);
	Location loc(location);
	if(loc.root() != d->root) {
		loc = resolve(location);
	}

	if(loc.isNull())
		return Handle();
	
	Handle handle = loc.resolve();
	d->locations.insert(handle, loc);
	return handle;
};

/**
 * Returns \a location relative to the root of the lookup
 * or an invalid location if the root of \a location was not
 * added to the lookup before.
 */
Location LocationLookup::resolve(const Location& location) const {
	Q_D(const LocationLookup);
	if(location.root() == d->root)
		return location;
	
	if(!d->locations.contains(location.root()))
		return Location();
	
	return d->locations.value(location.root()).relative(location.path());
}

}

}
