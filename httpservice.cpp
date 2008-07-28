#include "httpservice.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHttpRequestHeader>

#include <QDateTime>

#include <QtDebug>

#include "obuilder.h"
#include "resource.h"

#include "httpservice.moc"

namespace Orchid {


Request::Request(QIODevice* device) {
	m_device = device;
}


HttpServiceProcess::HttpServiceProcess(HttpService* service, QAbstractSocket* socket) {
	m_service = service;
	m_socket = socket;
	connect(m_socket, SIGNAL(readyRead()), this, SLOT(read()));
}

HttpServiceProcess::~HttpServiceProcess() {
	delete m_socket;
}

void HttpServiceProcess::read() {
	while(m_socket->canReadLine()) {
		QString line(m_socket->readLine());
		if(line == "\r\n") {
			QHttpRequestHeader requestHeader = QHttpRequestHeader(m_requestStr);

			qDebug() << requestHeader.path();

			process();
			Builder builder;
			builder.setDevice(m_socket);
			
			RestResource* res = dynamic_cast<RestResource*>(m_service->root());
			if(res) 
				res->methodGet(&builder);
// 			else
			
// 			m_socket->write("<pre>");
// 			m_socket->write(requestHeader.toString().toAscii());
// 			m_socket
// 			m_socket->write("</pre>");


			m_socket->disconnectFromHost();
			return;
		}
		m_requestStr += line;
	}
}

void HttpServiceProcess::process() {
	QHttpResponseHeader header(200, "OK");

	// TODO change this to local independant
	header.setValue("date", QDateTime::currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss") + " GMT");
	header.setValue("content-type", "text/html");
	m_socket->write(header.toString().toAscii());
}

HttpService::HttpService(int port) : m_port(port) {
	connect(&m_server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
	if(!m_server.listen(QHostAddress::Any, port)) {
		qWarning() << QString("Server failed to bind to port %1").arg(port);
	}
}

void HttpService::acceptConnection() {
	qDebug() << "acceptConnection";
	HttpServiceProcess* process = new HttpServiceProcess(this, m_server.nextPendingConnection());
}

}
