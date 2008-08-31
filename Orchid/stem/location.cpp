#include "location.h"

#include "resourcekeep.h"
#include "resource.h"

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

Handle Location::resource() const {
	// NOTE could be optimised by using references on string instead
	// of stringlist
	IResource* res = d->root.resource();
	QStringList path = d->path.split('/');
	if(path.isEmpty()) return d->root;
	
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
	return Location(d->root, d->path.left(p1Length) + '/' + rel.mid(p2Skip));
}

}

}
