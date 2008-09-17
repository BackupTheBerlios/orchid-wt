#include "location.h"

#include "resourcekeep.h"
#include "resource.h"

#include <QtCore/QHash>

#include <QtCore/QtDebug> // TODO to rem

namespace Orchid {

namespace Resource {

class LocationPrivate : public QSharedData {
public:
	Handle root;
	QString path;
};

Location::Location() {
	d = new LocationPrivate();
}

Location::Location(Handle root, const QString& path) {
	d = new LocationPrivate();
	d->root = root;
	d->path = path;
}

/**
 * \note Location has no idea of self links and parent links with
 * the exaption of relative(). "." or ".." will be interpreted as
 * resource names
 */
Location::Location(const Location& loc, const QString& rel) {
	d = loc.relative(rel).d;
}

/**
 * The copy constructor
 */
Location::Location(const Location& loc) {
	d = loc.d;
}

Location::~Location() {
}

/**
 * Returns true if this location has no root
 */
bool Location::isNull() const {
	return d->root.isEmpty();
}

Handle Location::root() const {
	return d->root;
}

QString Location::path() const {
	return d->path;
}

QString Location::name() const {
	return d->path.mid(d->path.lastIndexOf('/')+1);
}

Handle Location::resource() const {
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
 * \note "." at the begin of \p rel and only there is interepreted as
 * a self link. All occurences of ".." directly after the optional "."
 * will be interpreted as parent links.
 * 
 * \c Location(root, "foo/bar").relative("./../baz/./..") will result the
 * path "foo/baz/./.." where "." and ".." have no special meaning and
 * are interpreted as ordinary resource names.
 */
Location Location::relative(const QString &rel) const {
	int p2Skip = 0;
	int p1Length = d->path.length();
	while(QStringRef(&rel, p2Skip, 2) == "./") p2Skip += 2;
	while(QStringRef(&rel, p2Skip, 3) == "../") {
		if(p1Length < 0) return Location();
		p2Skip += 3;
		p1Length = d->path.lastIndexOf('/', p1Length)-1;
	}
	if(QStringRef(&rel, p2Skip, rel.length()-p2Skip) == "..") {
		if(p1Length < 0) return Location();
		p1Length = d->path.lastIndexOf('/', p1Length)-1;
	}
	if(p1Length < 0) return Location();
	if(p1Length == 0) return Location(d->root, rel.mid(p2Skip));
	return Location(d->root, d->path.left(p1Length) + '/' + rel.mid(p2Skip));
}

Location& Location::operator=(const Location &other) {
	d = other.d;
	return *this;
}


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

LocationLookup::LocationLookup() {
	d_ptr = new LocationLookupPrivate(this);
}

LocationLookup::LocationLookup(const Handle& root) {
	d_ptr = new LocationLookupPrivate(this);
	d_ptr->root = root;
}

LocationLookup::~LocationLookup() {
	delete d_ptr;
}

Handle LocationLookup::root() const {
	Q_D(const LocationLookup);
	return d->root;
}

void LocationLookup::setRoot(const Handle &root) {
	Q_D(LocationLookup);
	d->root = root;
	d->locations.clear();
}

Handle LocationLookup::addLocation(const Location& location) {
	Q_D(LocationLookup);
	Location loc(location);
	if(loc.root() != d->root) {
		loc = resolve(location);
	}

	if(loc.isNull())
		return Handle();
	
	Handle handle = loc.resource();
	d->locations.insert(handle, loc);
	return handle;
};

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
