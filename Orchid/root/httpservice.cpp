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

class HttpServiceRequest : public SimpleRequest {
public:
	HttpServiceRequest(const QHttpRequestHeader& header);
	RequestMethod method() const;
	void setMimeType(const QString &type);
	bool open(QIODevice::OpenMode mode);
private:
	QHttpRequestHeader m_requestHeader;
	QHttpResponseHeader m_responseHeader;
};

HttpServiceRequest::HttpServiceRequest(const QHttpRequestHeader& header) : m_requestHeader(header) {
	m_responseHeader.setValue("content-type", "text/html");
	setUrl(header.path());
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
			qDebug() << path;
			path.remove(0, 1);
			qDebug() << path;

			HttpServiceRequest request(requestHeader);
			request.setDevice(m_socket);
			
// 			Resource::IQueryable* res = dynamic_cast<Resource::IQueryable*>(Resource::Resource::locateUrl(m_service->root(), path).resource());
			Resource::Location location(m_service->root(), path);
			Resource::IQueryable* res = dynamic_cast<Resource::IQueryable*>(location.resource().resource());
			if(res) {
				res->query(&request);
			} else {
// 				request.setStatus(RequestNotFound);
			}
			
			m_socket->disconnectFromHost();
			return;
		}
		m_requestStr += line;
	}
}


HttpService::HttpService(int port) : Service(new HttpServicePrivate(this)) {
	Q_D(HttpService);
	d->port = port;
	connect(&d->server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
	if(!d->server.listen(QHostAddress::Any, port)) {
		qWarning() << QString("Server failed to bind to port %1").arg(port);
	}
}

void HttpService::acceptConnection() {
	Q_D(HttpService);
	qDebug() << "acceptConnection";
	HttpServiceProcess* process = new HttpServiceProcess(this, d->server.nextPendingConnection());
}

}
