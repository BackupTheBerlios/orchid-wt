#include "request.h"
#include "request_p.h" 

#include <QtCore/QUrl>

namespace Orchid {

class UrlResolver {
public:
	QUrl resolve(const QUrl& local);
};

QUrl UrlResolver::resolve(const QUrl& local) {
	return local;
}

Request::Request() {
    d_ptr = new RequestPrivate(this);
}

Request::Request(RequestPrivate* dd) {
	d_ptr = dd;
}

Request::~Request() {
	delete d_ptr;
}

RequestMethod Request::method() const { return UnknownMethod; }


SimpleRequest::SimpleRequest()
	: Request(new SimpleRequestPrivate(this))
{
}

QIODevice* SimpleRequest::readDevice() const {
	Q_D(const SimpleRequest);
	return d->readDevice;
}

QIODevice* SimpleRequest::writeDevice() const {
	Q_D(const SimpleRequest);
	return d->writeDevice;
}

void SimpleRequest::setDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

void SimpleRequest::setReadDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

void SimpleRequest::setWriteDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

qint64 SimpleRequest::readData(char* data, qint64 size) {
	Q_D(SimpleRequest);
	return d->readDevice->read(data, size);
}

qint64 SimpleRequest::writeData(const char* data, qint64 size) {
	Q_D(SimpleRequest);
	return d->writeDevice->write(data, size);
}

RequestMethod SimpleRequest::method() const {
	return GetMethod;
}

bool SimpleRequest::open(QIODevice::OpenMode mode) {
	Q_D(SimpleRequest);
	if(d->readDevice && mode.testFlag(QIODevice::ReadOnly) &&
		d->readDevice->openMode() == QIODevice::NotOpen)
	{
		if(!d->readDevice->open(mode & ~QIODevice::ReadOnly))
			return false;
	}
	if(d->writeDevice && mode.testFlag(QIODevice::WriteOnly) &&
		d->writeDevice->openMode() == QIODevice::NotOpen)
	{
		if(!d->writeDevice->open(mode & ~(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate)))
			return false;
	}
	
	return QIODevice::open(mode);
}


}
