#include "resourcekeep.h"

#include "resourcekeep.moc"

#include <QHash>
#include <QMutex>
#include <QAtomicInt>

#include "resource.h"

namespace Orchid {
namespace Resource {

struct KeepItem {
	KeepItem(Keep* keep, const QString& name);
	
	Keep* keep;
	QString name;
	IResource* resource;
	QMutex mutex;
	QAtomicInt handleRefs;
	int keepRefs;
	KeepingFlags flags;
};

KeepItem::KeepItem(Keep* keep, const QString& name)
	: keep(keep), name(name), resource(0), handleRefs(0), keepRefs(1), flags()
{
}


class KeepPrivate {
public:
	// methods to be used by the keep internaly
public:
	// methods to be used by the handles
	void releaseItem(KeepItem* item);
	bool initItem(KeepItem* item, IResource* resource, KeepingFlags flags);
public:
	QHash<QString, KeepItem*> items;
	QMutex mutex;
};

void KeepPrivate::releaseItem(KeepItem* item) {
	QMutexLocker locker(&mutex);
	
	// test whether the item is exclusive or uninitialised
	if(item->flags.testFlag(KeepExclusive) || !item->resource) {
		// other threads are allowed to take ownership
		item->mutex.unlock();
	}
	
	if(!item->resource) {
		// the item is still uninitialised
		// test whether there are no other threads waiting for item
		if(!item->keepRefs) {
			items.remove(item->name);
			delete item;
		}
	} else if(!item->flags.testFlag(KeepPersistant)) {
		// item doesn't need to stay when not in use
		// test whether there are no other threads waiting for item
		if(!item->keepRefs) {
			items.remove(item->name);
			delete item->resource;
			delete item;
		}
	}
}

bool KeepPrivate::initItem(KeepItem* item, IResource* resource, KeepingFlags flags) {
	// only 1 thread can init an item
	if(item->resource) return false;
	item->resource = resource;
	item->flags = flags;
	if(!flags.testFlag(KeepExclusive)) {
		item->mutex.unlock();
	}
	return true;
}

Keep::Keep() {
	d = new KeepPrivate;
}

Keep::~Keep() {
	QHash<QString, KeepItem*>::iterator it;
	for(it = d->items.begin(); it != d->items.end(); ++it) {
		delete it.value()->resource;
		delete it.value();
	}
	delete d;
}

Handle Keep::getHandle(const QString& name) {
	QMutexLocker locker(&d->mutex);
	KeepItem* item = d->items.value(name);
	if(!item) {
		item = new KeepItem(this, name);
		item->mutex.lock();
		d->items.insert(name, item);
	} else {
		// test whether
		if(!item->resource) {
			// tell other threads, that we are waiting for this resource
			item->keepRefs++; 
			// needs to be unlocked here to not block other threads operating on other items
			locker.unlock();
				
			// wait until another thread releases this unfinished resource
			// either by finishing it, or forgetting about it
			item->mutex.lock();
				
			// we have now the right on the previosly uninitialised resource
			// it either is initialised or not, in which case we allow this thread
			// to initialise it

			// if item is initialised and nonblocking release its mutex
			if(item->resource && !item->flags.testFlag(KeepExclusive))
				item->mutex.unlock();
				
			// relock to create the handle exclusively
			locker.relock();
			// remove the reference we took above
			item->keepRefs--;
		}
	}
	return Handle(item);
}

Handle Keep::tryGetHandle(const QString& name) {
	QMutexLocker locker(&d->mutex);
	KeepItem* item = d->items.value(name);
	if(!item) {
		item = new KeepItem(this, name);
		item->mutex.lock();
		d->items.insert(name, item);
	} else {
		// test whether this resource is uninitialised in which case other
		// threads are operating on it
		if(!item->resource) item = 0;
		// test whether this item is blocking
		else if(item->flags.testFlag(KeepExclusive)) {
			if(!item->mutex.tryLock()) item = 0;
		}
	}
	return Handle(item);
}


Handle::Handle() : m_item(0) {
}

Handle::Handle(const Handle& handle) : m_item(handle.m_item) {
	if(m_item) m_item->handleRefs.ref();
}

Handle::Handle(KeepItem* item) : m_item(item) {
	if(item) item->handleRefs.ref();
}

Handle::~Handle() {
	if(m_item && !m_item->handleRefs.deref()) {
		if(m_item->keep) {
			// is keeped
			m_item->keep->d->releaseItem(m_item);
		} else {
			// is not keeped
			delete m_item->resource;
			delete m_item;
		}
	}
}

bool Handle::isNull() const {
	return !m_item;
}

bool Handle::isEmpty() const {
	if(!m_item) return true;
	return !m_item->resource;
}

IResource* Handle::resource() const {
	if(!m_item) return 0;
	return m_item->resource;
}

bool Handle::init(IResource* resource, KeepingFlags flags) {
	if(!m_item) {
		// is not keeped => no flags allowed
		if(flags != KeepingFlags())
			return false;
		m_item = new KeepItem(0, QString());
		m_item->resource = resource;
		m_item->handleRefs.ref();
		return true;
	}
	
	if(!m_item->keep) return false;
	return m_item->keep->d->initItem(m_item, resource, flags);
}

QString Handle::name() const {
	if(!m_item) return QString();
	
	return m_item->name;
}

Handle& Handle::operator=(const Handle& other) {
	if(m_item == other.m_item) return *this;
	
	if(other.m_item)
		other.m_item->handleRefs.ref();
	if(m_item && !m_item->handleRefs.deref()) {
		if(m_item->keep) {
			// is keeped
			m_item->keep->d->releaseItem(m_item);
		} else {
			// is not keeped
			delete m_item->resource;
			delete m_item;
		}
	}
	m_item = other.m_item;
}

bool Handle::operator==(const Handle &other) const {
	if(!m_item) {
		if(!other.m_item) return true;
		return false;
	}
	if(!other.m_item) return false;
	return m_item->resource == other.m_item->resource;
}

}
}

uint qHash(const Orchid::Resource::Handle &handle) {
	return ::qHash(handle.resource());
}
