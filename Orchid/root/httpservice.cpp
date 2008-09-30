/*
 * This file is part of the Orchid libraries
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "httpservice.h"
#include "httpservice_p.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHttpRequestHeader>

#include <QDateTime>

#include <QtDebug>

#include <stem/request.h>
#include <stem/resource.h>
#include <stem/resourcekeep.h>
#include <stem/location.h>

#include "httpservice.moc"
#include "httpservice_p.moc"

namespace Orchid {

/**
 * \class HttpService
 *
 * \brief HttpService provides an HTTP server for resources.
 *
 * \note This server is only thought to be used for testing
 * purposes. It was not optimised for usage in real world
 * scenarios.
 */

class HttpServiceRequest : public SimpleRequest {
public:
	HttpServiceRequest(const QHttpRequestHeader& header, const Resource::Location& location);
	RequestMethod method() const;
	void setMimeType(const QString &type);
	bool open(QIODevice::OpenMode mode);
	void setRootUrl(const QString &url);
	QString url(const Resource::Location &location) const;
private:
	QString m_rootUrl;
	QHttpRequestHeader m_requestHeader;
	QHttpResponseHeader m_responseHeader;
};

HttpServiceRequest::HttpServiceRequest(const QHttpRequestHeader& header, const Resource::Location &location) : m_requestHeader(header) {
	m_responseHeader.setValue("content-type", "text/html");
	setRoot(location.root());
	setLocation(location);
}

RequestMethod HttpServiceRequest::method() const {
	if(m_requestHeader.method() == "GET") return HttpGetMethod;
	if(m_requestHeader.method() == "POST") return HttpPostMethod;
	if(m_requestHeader.method() == "PUT") return HttpPutMethod;
	if(m_requestHeader.method() == "DELETE") return HttpDeleteMethod;
	return UnknownMethod;
}

void HttpServiceRequest::setMimeType(const QString& type) {
	qDebug() << "setMimeType";
	m_responseHeader.setValue("content-type", type);
}

bool HttpServiceRequest::open(QIODevice::OpenMode mode) {
	Q_ASSERT(mode.testFlag(QIODevice::WriteOnly));
	if(openMode() != QIODevice::NotOpen) return false;
	
// 	QIODevice* input = readDevice();
// 	if(mode.testFlag(QIODevice::ReadOnly) && input->openMode() == QIODevice::NotOpen) {
// 		if(!input->open(mode & ~(QIODevice::WriteOnly | QIODevice::Append  | QIODevice::Truncate)))
// 			return false;
// 	}
	QIODevice* output = writeDevice();
// 	if(output->openMode() == QIODevice::NotOpen) {
// 		if(!input->open(mode & ~QIODevice::ReadOnly))
// 			return false;
// 	}
	if(!SimpleRequest::open(mode)) return false;
	
	
	m_responseHeader.setStatusLine(200, "OK");

	// TODO change this to local independant
	m_responseHeader.setValue("date", QDateTime::currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss") + " GMT");
	output->write(m_responseHeader.toString().toAscii());

// 	return QIODevice::open(mode);
	return true;
}

void HttpServiceRequest::setRootUrl(const QString &url) {
	if(url.endsWith('/')) {
		m_rootUrl = url;
	} else {
		m_rootUrl = url + '/';
	}
}

QString HttpServiceRequest::url(const Resource::Location &location) const {
	return m_rootUrl + resolve(location).path();
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

			QString path = requestHeader.path();
			path.remove(0, 1);

			Resource::Location location(m_service->root(), path);
			HttpServiceRequest request(requestHeader, location);
			request.setDevice(m_socket);
			request.setRootUrl(QString("http://localhost:%1/").arg(m_service->port()));
			
			request.query();
			
			m_socket->disconnectFromHost();
			return;
		}
		m_requestStr += line;
	}
}


/**
 * Constructs a new service with a http server that listens for
 * incoming connections on \a port.
 */
HttpService::HttpService(int port) : Service(new HttpServicePrivate(this)) {
	Q_D(HttpService);
	d->port = port;
	connect(&d->server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
	if(!d->server.listen(QHostAddress::Any, port)) {
		qWarning() << QString("Server failed to bind to port %1").arg(port);
	}
}

/**
 * Returns the port the server is listening on.
 */
int HttpService::port() const {
	Q_D(const HttpService);
	return d->port;
}

/**
 * \internal
 */
void HttpService::acceptConnection() {
	Q_D(HttpService);
	qDebug() << "acceptConnection";
	HttpServiceProcess* process = new HttpServiceProcess(this, d->server.nextPendingConnection());
}

}
