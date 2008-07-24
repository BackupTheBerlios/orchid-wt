#ifndef _RESOURCEKEEP_H_
#define _RESOURCEKEEP_H_

#include <QObject>

namespace Orchid {
namespace Resource {

class Resource;

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
	Resource* resource() const;
	bool init(Resource* resource, KeepingFlags flags = KeepingFlags());
	QString name() const;
	Handle& operator=(const Handle& other);
private:
	Handle(KeepItem* item);
private:
	KeepItem* m_item;
};

}
}

#endif
