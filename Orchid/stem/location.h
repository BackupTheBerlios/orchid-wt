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
	Location(const Location& loc, const QString& rel);
	Location(const Location& loc);
	~Location();
public:
	bool isNull() const;
	Handle resource() const;
	Location relative(const QString& rel) const;
private:
	QSharedDataPointer<LocationPrivate> d;
};

class LocationLookupPrivate;
class LocationLookup {
public:
	LocationLookup(Handle root);
public:
	Handle addLocation(const Location& loc);
	Location locate(const Location& loc);
};

}

}

#endif
