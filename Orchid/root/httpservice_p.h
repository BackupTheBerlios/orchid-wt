#ifndef _ORCHID_HTTPSERVICE_PRIVATE_H_
#define _ORCHID_HTTPSERVICE_PRIVATE_H_

#include "service_p.h"

#include <QtCore/QString>
#include <QtNetwork/QTcpServer>

class HttpService;
class QAbstractSocket;

namespace Orchid {

class HttpServiceProcess : public QObject {
	Q_OBJECT
public:
	HttpServiceProcess(HttpService* service, QAbstractSocket* socket);
	~HttpServiceProcess();
public slots:
	void read();
private:
	HttpService* m_service;
	QString m_requestStr;
	QAbstractSocket* m_socket;
};

class HttpServicePrivate : public ServicePrivate {
public:
	HttpServicePrivate(HttpService* service) : ServicePrivate(service) {}
public:
	int port;
	QTcpServer server;
private:
	Q_DECLARE_PUBLIC(HttpService);
};

}

#endif
