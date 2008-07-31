#ifndef _ORCHID_SERVICE_H_
#define _ORCHID_SERVICE_H_

#include <QObject>

#include "resourcekeep.h"

namespace Orchid {

class Service : public QObject {
public:
	Service();
public:
	void setRoot(const Resource::Handle& root);
	Resource::Handle root() const;
private:
	Resource::Handle m_root;
};


}

#endif
