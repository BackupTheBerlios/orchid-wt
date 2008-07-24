#ifndef _ORCHID_SERVICE_H_
#define _ORCHID_SERVICE_H_

#include <QObject>

namespace Orchid {

namespace Resource {
class Resource;
}

class Service : public QObject {
public:
	Service();
public:
	void setRoot(Resource::Resource* root);
	Resource::Resource* root() const;
private:
	Resource::Resource* m_root;
};


}

#endif
