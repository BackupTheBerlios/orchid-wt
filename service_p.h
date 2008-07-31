#ifndef _ORCHID_SERVICE_PRIVATE_H_
#define _ORCHID_SERVICE_PRIVATE_H_

#include "resourcekeep.h"

namespace Orchid {

class Service;
class ServicePrivate {
public:
	ServicePrivate(Service* service) : q_ptr(service) {}
public:
	Resource::Handle root;
private:
	Q_DECLARE_PUBLIC(Service);
protected:
	Service* q_ptr;
};

}

#endif
