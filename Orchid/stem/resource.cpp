#include "resource.h" 
#include "request.h"
#include "resourcekeep.h"

#include "resourcebase_p.h"

#include <QTextStream>
#include <QMutex>

#include <QtDebug>


namespace Orchid {

static QMutex interfaceRegMutex;
static QHash<QByteArray,int> interfaceList;

int regInterface(const char *name) {
	QByteArray key(name);
	QMutexLocker locker(&interfaceRegMutex);
	static int nextId = 1;

	int id = interfaceList.value(key);
	if(!id) {
		id = nextId++;
		interfaceList.insert(key, id);
	}

	return id;
}

class ContainerResourcePrivate : public Resource::BasePrivate {
	Q_DECLARE_PUBLIC(ContainerResource)
public:
	ContainerResourcePrivate(ContainerResource* resource);
private:
	QStringList m_childs;
};

ContainerResourcePrivate::ContainerResourcePrivate(ContainerResource* resource) 
	: BasePrivate(resource)
{ }

ContainerResource::ContainerResource() : Base(new ContainerResourcePrivate(this)) {
}

ContainerResource::~ContainerResource() {
}


bool ContainerResource::addResource(const QString& name, Resource::Base* res, Resource::Ownership ownership) {
	Q_D(ContainerResource);
	Resource::Handle handle = keep()->acquireHandle(name);
	if(!handle.isEmpty()) return false;
	handle.init(res, ownership, Resource::KeepPersistant);
	d->m_childs << name;
	return true;
}

bool ContainerResource::remove(const QString &name) {
	return keep()->reset(name);
}

bool ContainerResource::removeAll() {
	Q_D(ContainerResource);
	keep()->resetAll();
	d->m_childs.clear();
	return true;
}

QStringList ContainerResource::childs() const {
	Q_D(const ContainerResource);
	return d->m_childs;
}

Orchid::Resource::Handle ContainerResource::child(const QString& name) {
	Q_D(ContainerResource);
	Resource::Handle handle = keep()->acquireHandle(name);
	if(handle.isEmpty())
		return Resource::Handle();
	return handle;
}

}
