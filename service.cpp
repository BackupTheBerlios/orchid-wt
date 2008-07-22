#include "service.h" 

namespace Orchid {

Service::Service() {
	m_root = 0;
}

void Service::setRoot(Resource* root) {
	m_root = root;
}

Resource* Service::root() const {
	return m_root;
}

}
