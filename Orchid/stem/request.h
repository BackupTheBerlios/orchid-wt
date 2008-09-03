#ifndef _ORCHID_REQUEST_H_
#define _ORCHID_REQUEST_H_

#include <QtCore/qnamespace.h>
#include <QtCore/QIODevice>

class QIODevice;
class QUrl;

namespace Orchid {

namespace Resource {
	class Location;
	class Handle;
}

class RequestPrivate;

enum RequestStatus {
	RequestTerminated,
	RequestNotFound,
	RequestMethodNotAllowed
};

enum RequestMethod {
	UnknownMethod     = 0x0000,
	GetMethod         = 0x0001,
	// HttpMethodFlag = 0x0040
	HttpGetMethod     = 0x0041,
	HttpPostMethod    = 0x0042,
	HttpHeadMethod    = 0x0044,
	HttpPutMethod     = 0x0048,
	HttpDeleteMethod  = 0x0050,
	HttpOptionsMethod = 0x0060,
};

class Request : public QIODevice {
public:
	Request();
	virtual ~Request();
public:
	Resource::Handle addLocation(const Resource::Location& location);
	Resource::Location resolve(const Resource::Location& location) const;
	Resource::Location location() const;
	Resource::Handle resource() const;
	bool query();
public:
// 	RequestStatus status() const;
// 	virtual bool setStatus(RequestStatus reason, QVariant arg);
	virtual RequestMethod method() const;
	virtual void setMimeType(const QString &type);
	virtual QString url(const Resource::Location& location) const;
protected:
	Request(RequestPrivate* dd);
private:
	Q_DECLARE_PRIVATE(Request)
protected:
	void setRoot(const Resource::Handle& root);
	void setLocation(const Resource::Location& location);
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
	virtual RequestMethod method() const;
	virtual bool open(QIODevice::OpenMode mode);
protected:
	SimpleRequest(SimpleRequestPrivate* dd);
private:
	Q_DECLARE_PRIVATE(SimpleRequest);
};

}

#endif
