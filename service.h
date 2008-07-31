#ifndef _ORCHID_SERVICE_H_
#define _ORCHID_SERVICE_H_

#include <QObject>

namespace Orchid {

namespace Resource {
class Handle;
}

class ServicePrivate;

class Service : public QObject {
	Q_OBJECT
public:
	Service();
public:
	void setRoot(const Resource::Handle& root);
	Resource::Handle root() const;
protected:
	Service(ServicePrivate* dd);
private:
	Q_DECLARE_PRIVATE(Service)
protected:
	ServicePrivate* d_ptr;

};

}

#endif
