#include "service.h" 

namespace Orchid {

Service::Service() {
}

void Service::setRoot(const Resource::Handle& root) {
	m_root = root;
}

Resource::Handle Service::root() const {
	return m_root;
}

}
