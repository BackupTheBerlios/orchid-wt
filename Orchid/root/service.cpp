#include "service.h"
#include "service_p.h"

#include "service.moc"

namespace Orchid {

Service::Service() : d_ptr(new ServicePrivate(this)) {
}

Service::Service(ServicePrivate* dd) : d_ptr(dd) {
}

void Service::setRoot(const Resource::Handle& root) {
	Q_D(Service);
	d->root = root;
}

Resource::Handle Service::root() const {
	Q_D(const Service);
	return d->root;
}

}
