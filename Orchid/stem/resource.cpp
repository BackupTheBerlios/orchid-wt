#include "resource.h" 
#include "request.h"
#include "resourcekeep.h"

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

class ContainerResourcePrivate {
public:
	ContainerResourcePrivate(ContainerResource* resource);
protected:
	ContainerResource* q_ptr;
private:
	Q_DECLARE_PUBLIC(ContainerResource)
	Orchid::Resource::Keep m_keep;
	QHash<QString, Orchid::Resource::Handle> m_childs;
};

ContainerResourcePrivate::ContainerResourcePrivate(ContainerResource* resource)
	: q_ptr(resource)
{ }

ContainerResource::ContainerResource() {
	d_ptr = new ContainerResourcePrivate(this);
}

ContainerResource::~ContainerResource() {
	delete d_ptr;
}


bool ContainerResource::addResource(const QString& name, Resource::IResource* res, Resource::Ownership ownership) {
	Q_D(ContainerResource);
	Resource::Handle handle = d->m_keep.getHandle(name);
	handle.init(res, ownership, Resource::KeepPersistant);
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
