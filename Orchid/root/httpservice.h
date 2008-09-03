#ifndef _ORCHID_HTTPSERVICE_H_
#define _ORCHID_HTTPSERVICE_H_

#include "service.h"

namespace Orchid {

class HttpServicePrivate;
class HttpService : public Service {
	Q_OBJECT
public:
	HttpService(int port = 80);
public:
	int port() const;
private slots:
	void acceptConnection();
private:
	Q_DECLARE_PRIVATE(HttpService);
};

}

#endif
