#include "resource.h" 
#include "request.h"
#include "resourcekeep.h"

#include <QTextStream>

#include <QtDebug>

namespace Orchid {

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


bool ContainerResource::addResource(const QString& name, Resource::IResource* res) {
	Q_D(ContainerResource);
	Orchid::Resource::Handle handle = d->m_keep.getHandle(name);
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

SimpleTextResource::SimpleTextResource(const QString& text) {
	m_text = text;
}

void SimpleTextResource::query(Request* request) {
	qDebug() << "query";
	if(!request->open(QIODevice::ReadWrite)) return;
	QTextStream stream(request);
	stream << m_text;
}

}
