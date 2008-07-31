#ifndef _ORCHID_REQUEST_H_
#define _ORCHID_REQUEST_H_

#include <QtCore/qnamespace.h>
#include <QtCore/QIODevice>

class QIODevice;
class QUrl;

namespace Orchid {

class RequestPrivate;

enum RequestStatus {
	RequestTerminated,
	RequestNotFound,
	RequestMethodNotAllowed
};

enum RequestMethod {
	UnknownMethod     = 0x0000,
	HttpGetMethod     = 0x0001,
	HttpPostMethod    = 0x0002,
	HttpHeadMethod    = 0x0004,
	HttpPutMethod     = 0x0008,
	HttpDeleteMethod  = 0x0010,
	HttpOptionsMethod = 0x0020,
};

class Request : public QIODevice {
public:
	Request();
	virtual ~Request();

	// makes a global url from an local url
	QUrl resolve(const QUrl& url);
public:
// 	RequestStatus status() const;
// 	virtual bool setStatus(RequestStatus reason, QVariant arg);
	virtual RequestMethod method() const;
protected:
	Request(RequestPrivate* dd);
private:
	Q_DECLARE_PRIVATE(Request)
protected:
	RequestPrivate* d_ptr;
};

class SimpleRequestPrivate;

// simple request forwards the request to other QIODevice instances
class SimpleRequest : public Request {
public:
	SimpleRequest();
public:
	QIODevice* readDevice() const;
	QIODevice* writeDevice() const;
	void setDevice(QIODevice* device);
	void setReadDevice(QIODevice* device);
	void setWriteDevice(QIODevice* device);
	qint64 readData(char*, qint64);
	qint64 writeData(const char*, qint64);

protected:
	SimpleRequest(SimpleRequestPrivate* dd);
private:
	Q_DECLARE_PRIVATE(SimpleRequest);
};

}

#endif
