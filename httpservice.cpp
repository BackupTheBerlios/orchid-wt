#include "httpservice.h"

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHttpRequestHeader>

#include <QDateTime>

#include <QtDebug>

#include "request.h"
#include "resource.h"
#include "resourcekeep.h"

#include "httpservice.moc"

namespace Orchid {

class HttpServiceRequest : public SimpleRequest {
public:
	HttpServiceRequest(const QHttpRequestHeader& header);
	RequestMethod method() const;
	bool open(QIODevice::OpenMode mode);
private:
	QHttpRequestHeader m_header;
};

HttpServiceRequest::HttpServiceRequest(const QHttpRequestHeader& header) : m_header(header) {
}

RequestMethod HttpServiceRequest::method() const {
	if(m_header.method() == "GET") return HttpGetMethod;
	if(m_header.method() == "POST") return HttpPostMethod;
	if(m_header.method() == "PUT") return HttpPutMethod;
	if(m_header.method() == "DELETE") return HttpDeleteMethod;
	return UnknownMethod;
}

bool HttpServiceRequest::open(QIODevice::OpenMode mode) {
	Q_ASSERT(mode.testFlag(QIODevice::WriteOnly));
	if(openMode() != QIODevice::NotOpen) return false;
	
	QIODevice* input = readDevice();
	if(mode.testFlag(QIODevice::ReadOnly) && input->openMode() == QIODevice::NotOpen) {
		if(!input->open(mode & ~(QIODevice::WriteOnly | QIODevice::Append  | QIODevice::Truncate)))
			return false;
	}
	QIODevice* output = writeDevice();
	if(output->openMode() == QIODevice::NotOpen) {
		if(!input->open(mode & ~QIODevice::ReadOnly))
			return false;
	}
	
	QHttpResponseHeader header(200, "OK");

	// TODO change this to local independant
	header.setValue("date", QDateTime::currentDateTime().toString("ddd, dd MMM yyyy hh:mm:ss") + " GMT");
	header.setValue("content-type", "text/html");
	output->write(header.toString().toAscii());
	
	return QIODevice::open(mode);
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
			
			Resource::IQueryable* res = dynamic_cast<Resource::IQueryable*>(Resource::Resource::locateUrl(m_service->root(), path).resource());
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