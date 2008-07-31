#ifndef _HTTPSERVICE_H_
#define _HTTPSERVICE_H_

#include "service.h"
#include <QtNetwork/QTcpServer>

class QAbstractSocket;

namespace Orchid {

class HttpServiceDaemon;
class HttpService;

// class Request {
// public:
// 	Request(QIODevice* device);
// public:
// 	QIODevice* device() const;
// private:
// 	QIODevice* m_device;
// };

class HttpServiceProcess : public QObject {
	Q_OBJECT
public:
// 	HttpServiceProcess(int socketDescriptor);
	HttpServiceProcess(HttpService* service, QAbstractSocket* socket);
	~HttpServiceProcess();
public slots:
	void read();
private:
	HttpService* m_service;
	QString m_requestStr;
	QAbstractSocket* m_socket;
};

class HttpService : public Service {
	Q_OBJECT
public:
	HttpService(int port = 80);
private slots:
	void acceptConnection();
private:
	int m_port;
// 	HttpServiceDaemon* m_daemon;
	QTcpServer m_server;
};

}

#endif
