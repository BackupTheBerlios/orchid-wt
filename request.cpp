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

QIODevice* Request::device() const {
	Q_D(const Request);
	return d->device;
}

void Request::setDevice(QIODevice* device) {
	Q_D(Request);
	d->device = device;
}

}
