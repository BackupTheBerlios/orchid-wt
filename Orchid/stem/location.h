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
	Handle root() const;
	QString path() const;
	QString name() const;
	Handle resource() const;
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
