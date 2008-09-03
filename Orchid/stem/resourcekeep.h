#ifndef _RESOURCEKEEP_H_
#define _RESOURCEKEEP_H_

#include <QObject>

namespace Orchid {
namespace Resource {

class IResource;

enum KeepingFlag {
	KeepPersistant = 1,
	KeepExclusive = 2,
};
Q_DECLARE_FLAGS(KeepingFlags, KeepingFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(KeepingFlags);

class Handle;
class KeepItem;
class KeepPrivate;
class Keep : public QObject {
	Q_OBJECT
	friend class ::Orchid::Resource::Handle;
public:
	Keep();
	~Keep();
public:
	Handle getHandle(const QString& name);
	Handle tryGetHandle(const QString& name);
private:
	KeepPrivate* d;
};

class Handle {
	friend class ::Orchid::Resource::Keep;
public:
	Handle();
	Handle(const Handle& handle);
	~Handle();
public:
	bool isNull() const;
	bool isEmpty() const;
	IResource* resource() const;
	bool init(IResource* resource, KeepingFlags flags = KeepingFlags());
	QString name() const;
	Handle& operator=(const Handle& other);
	bool operator==(const Handle& other) const;
	inline bool operator!=(const Handle& other) const;
private:
	explicit Handle(KeepItem* item);
private:
	KeepItem* m_item;
};

inline bool Handle::operator!=(const Handle& other) const {
	return !operator==(other);
}

}
}

uint qHash(const Orchid::Resource::Handle& handle);

#endif
