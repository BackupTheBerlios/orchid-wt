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
	QHash<QString, Orchid::Resource::Handle> m_childs;
};

ContainerResourcePrivate::ContainerResourcePrivate(ContainerResource* resource) 
	: BasePrivate(resource)
{ }

ContainerResource::ContainerResource() {
	d_ptr = new ContainerResourcePrivate(this);
}

ContainerResource::~ContainerResource() {
	delete d_ptr;
}


bool ContainerResource::addResource(const QString& name, Resource::Base* res) {
	Q_D(ContainerResource);
	Resource::Handle handle = keep()->acquireHandle(name);
	handle.init(res, Orchid::Resource::KeepPersistant);
	d->m_childs.insert(name, handle);
	return true;
}

QStringList ContainerResource::childs() const {
	Q_D(const ContainerResource);
	return d->m_childs.keys();
}

Orchid::Resource::Handle ContainerResource::child(const QString& name) {
	Q_D(ContainerResource);
	return d->m_childs.value(name);
}

}
