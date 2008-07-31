#ifndef _HTTPSERVICE_H_
#define _HTTPSERVICE_H_

#include "service.h"
#include <QtNetwork/QTcpServer>

namespace Orchid {

class HttpService : public Service {
	Q_OBJECT
public:
	HttpService(int port = 80);
private slots:
	void acceptConnection();
private:
	int m_port;
	QTcpServer m_server;
};

}

#endif
