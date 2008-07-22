#ifndef _ORCHID_SERVICE_H_
#define _ORCHID_SERVICE_H_

#include <QObject>

namespace Orchid {

class Resource;

class Service : public QObject {
public:
	Service();
public:
	void setRoot(Resource* root);
	Resource* root() const;
private:
	Resource* m_root;
};


}

#endif
