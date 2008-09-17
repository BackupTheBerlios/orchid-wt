#include "resourcekeep.h"

#include <QHash>
#include <QMutex>
#include <QAtomicInt>

#include "resource.h"


// TODO make theoretical proof of the keep being thread-safe

namespace Orchid {
namespace Resource {

/*!
 * \class Keep
 *
 * \brief The Keep class can be used to manage a list of resources.
 *
 * The Keep is a container class that stores associoation of names and resources.
 * It allows to access resources from different threads in a managed way and to
 * hold resources for the time they are needed.
 
 *
 * It adds some managment to them in that it is able to manage them in a thread-safe way
 * 
 * \sa Base
 */


struct KeepItem {
	KeepItem(Keep* keep, const QString& name);
	~KeepItem();
	
	Keep* keep;
	QString name;
	Base* resource;
	
	QMutex notFreeMutex; // locked when no new acquire is allowed because of not null handleRefs
	                     // this i case when the item is not initialized
	
	QAtomicInt handleRefs; // number of handles + keepWithHandles
	QAtomicInt waitRefs; // number of aquire calls waiting for this Item;
	
	KeepingFlags flags;
};

KeepItem::KeepItem(Keep* keep, const QString& name)
	: keep(keep), name(name), resource(0), handleRefs(0), waitRefs(0), flags()
{
}

KeepItem::~KeepItem() {
	delete resource;
}


class KeepPrivate {
	friend class Keep;
public:
	KeepPrivate(Keep *keep);
public:
	// methods to be used by the keep internaly
	void setHandle(KeepItem *item);
public:
	// methods to be used by the handles
	void initItem(KeepItem *item, Base *resource, KeepingFlags flags);
	void releaseItem(KeepItem* item);
private:
	Keep *q;
	KeepItem *handle;
	int referencedItems;
	QHash<QString, KeepItem*> items;
	QMutex mutex;
};

KeepPrivate::KeepPrivate(Keep *keep) {
	q = keep;
	handle = 0;
	referencedItems = 0;
}

void KeepPrivate::setHandle(KeepItem *item) {
	handle = item;
	
	if(referencedItems)
		handle->handleRefs.ref();
}

void KeepPrivate::initItem(KeepItem* item, Base* resource, KeepingFlags flags) {
	// Only 1 thread is allowed to have an uninitialised item -> no locking required
	
	// calling initItem is only allowed for uninitialized items
	Q_ASSERT(!item->resource);
	
	// set values
	item->resource = resource;
	// flags can be set even without a keep as they are only used by the keep
	item->flags = flags;
	
	if(flags.testFlag(KeepPersistant)) {
		// the item is persistant and will never be deleted...
		// add a handle ref so that user has to reset it
		item->handleRefs.ref();
	}
	
	// make the items keep register itself to this item
	item->resource->keep()->d->setHandle(item);
	
	// The item is now initialized, allow further acquires
	item->notFreeMutex.unlock();
}

void KeepPrivate::releaseItem(KeepItem* item) {
	// Secure this operation
	QMutexLocker locker(&mutex);
	
	// This method is called as item has no more Handles
	
	if(!item->resource) {
		// the item is still uninitialised
		// other threads are allowed to acquire it
		item->notFreeMutex.unlock();
		
		if(!item->waitRefs) {
			// no other thread tries to acquire it now, delete it
			items.remove(item->name);
			delete item;
			item = 0;
		}
	} else {
		// we have an initialized item
		
		if(!item->flags.testFlag(KeepLonglifed)) {
			// persistant items will never have zero handles so no need to care about them
			// item is not longlifed, so it can be released when no acquires wait for it
			if(!item->waitRefs) {
				// no acquires wait for this item, remove it
				items.remove(item->name);
				delete item;
				item = 0;
			}
		}
	}
	// if item was not deleted yet it is either about to be aquired from another thread
	// ot it is longlifed and therefore will not be released until death of keep
}


Keep::Keep() {
	d = new KeepPrivate(this);
}

Keep::~Keep() {
	// make sure that no item stays in the keep
	resetAll();

	delete d;
}

Handle Keep::acquireHandle(const QString& name) {
	// Secure this operation
	QMutexLocker locker(&d->mutex);
	KeepItem* item = d->items.value(name);
	if(!item) {
		item = new KeepItem(this, name);
		// Lock because item is not initialized
		item->notFreeMutex.lock();
		d->items.insert(name, item);
	} else {
		// We have allready an item
		if(item->resource == 0) {
			// Acquire can not finish as long as there are allready handles for it
			// Tell releaseItem that we are waiting for this item
			item->waitRefs.ref();
			
			// We are now waiting for the item and do nothing else
			// let other threads do their work
			locker.unlock();
			
			// Now wait for the item to be ready
			item->notFreeMutex.lock();
			
			// Ok got exclusive rigth on the item, now go on
			locker.relock();
			
			// We are not waiting anymore
			item->waitRefs.deref();
			
			if(!item->keep) {
				// Item was reset, we need a new item
				
				// Take care of old item
				if(!(item->handleRefs || item->waitRefs)) {
					// There are no refs to this item, we can savely delet it
					delete item;
				}
				
				// Create a new item
				item = new KeepItem(this, name);
				// Lock because item is not initialized
				item->notFreeMutex.lock();
				d->items.insert(name, item);
			}
			
			if(item->resource != 0) {
				// situation has changed while waiting, acquire now allways allowed
				item->notFreeMutex.unlock();
			}
		}
	}
	return Handle(item);
}

Handle Keep::tryAcquireHandle(const QString& name) {
	// Secure this operation
	QMutexLocker locker(&d->mutex);
	KeepItem* item = d->items.value(name);
	if(!item) {
		item = new KeepItem(this, name);
		// Lock because item is not initialized
		item->notFreeMutex.lock();
		d->items.insert(name, item);
	} else {
		// We have allready an item
		if(item->resource == 0) {
			// Acquire can not finish as long as there are allready handles for it
			// There are obviously other handles as empty items don't outlive their handle
			// Don't wait and forget about this item
			item = 0;
		}
	}
	return Handle(item);
}

void Keep::reset(const QString &name) {
	// Secure this operation
	QMutexLocker locker(&d->mutex);
	
	// Find the right item
	QHash<QString, KeepItem*>::iterator it = d->items.find(name);
	if(it == d->items.end()) return;
	
	// Ok we have the item
	KeepItem *item = it.value();
	
	if(!item->resource) {
		// item is not initialized yet so no ned to reset it
		return;
	}
	
	// Move item out of the keep
	d->items.erase(it);
	item->keep = 0;
	
	// notFreeMutex is only locked for not initialized items
	// which is never the case here, so ignore it

	if(item->flags.testFlag(KeepPersistant)) {
		// item is persistant, we need to manually remove its persistants reference
		item->handleRefs.deref();
	}

	// Check whether this item is used somewhere else
	if(!(item->handleRefs || item->waitRefs)) {
		// There are no refs to this item, we can savely delet it
		delete item;
	}
}

void Keep::resetAll() {
	QMutexLocker locker(&d->mutex);
	QHash<QString, KeepItem*>::iterator it;
	for(it = d->items.begin(); it != d->items.end(); ++it) {
		KeepItem *item = it.value();
		
		if(!item->resource) {
			// item is not initialized yet so no ned to reset it
			continue;
		}
		
		// Move item out of the keep
		item->keep = 0;
		
		// notFreeMutex is only locked for not initialized items
		// which is never the case here, so ignore it

		if(item->flags.testFlag(KeepPersistant)) {
			// item is persistant, we need to manually remove its persistants reference
			item->handleRefs.deref();
		}

		if(!(item->handleRefs || item->waitRefs)) {
			// There are no refs to this item, we can savely delet it
			delete item;
		}
	}
	// Clear the item-hash
	d->items.clear();
}


Handle::Handle() : m_item(0) {
}

Handle::Handle(const Handle& handle) : m_item(handle.m_item) {
	// add reference to the item
	if(m_item) m_item->handleRefs.ref();
}

Handle::Handle(KeepItem* item) : m_item(item) {
	// add reference to the item
	if(item) item->handleRefs.ref();
}

Handle::~Handle() {
	if(m_item && !m_item->handleRefs.deref()) {
		// no other handles exist for this item
		if(m_item->keep) {
			// item is keeped, let the keep release it
			m_item->keep->d->releaseItem(m_item);
		} else {
			// item is not keeped, delete it
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

Base* Handle::resource() const {
	if(!m_item) return 0;
	return m_item->resource;
}

void Handle::init(Base* resource, KeepingFlags flags) {
	if(!m_item) {
		// this handle doesn't is not bound to an keep
		
		// keeples handles can't have KeepingFlags
		Q_ASSERT(flags == KeepingFlags());

		m_item = new KeepItem(0, QString());
		m_item->resource = resource;
		m_item->handleRefs.ref();
	} else {
		// calling init for initialized items is not allowed
		Q_ASSERT(!m_item->resource);
		
		// uninitialized handles with items require a keep
		Q_ASSERT(m_item->keep);

		// let the keep initialize the item
		m_item->keep->d->initItem(m_item, resource, flags);
	}
}

QString Handle::name() const {
	if(!m_item) return QString();
	
	return m_item->name;
}

Handle& Handle::operator=(const Handle& other) {
	if(m_item == other.m_item) return *this;
	
	if(other.m_item) {
		// add reference to new item
		other.m_item->handleRefs.ref();
	}
	
	// take care of old item
	if(m_item && !m_item->handleRefs.deref()) {
		// no other handles exist for this item
		if(m_item->keep) {
			// item is keeped, let the keep release it
			m_item->keep->d->releaseItem(m_item);
		} else {
			// item is not keeped, delete it
			delete m_item;
		}
	}
	
	// set new item
	m_item = other.m_item;
	return *this;
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
