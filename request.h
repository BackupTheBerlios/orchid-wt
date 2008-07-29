#ifndef _ORCHID_BUILDER_H_
#define _ORCHID_BUILDER_H_

#include <QtCore/qnamespace.h>

class QIODevice;
class QUrl;

namespace Orchid {

class RequestPrivate;

enum AbortReason {
	HTTPMethodNotAllowed
};

class Request {
public:
	Request();
	virtual ~Request();
public:
	QIODevice* device() const;
	virtual void setDevice(QIODevice* device);

	// makes a global url from an local url
	QUrl resolve(const QUrl& url);

public:
	void failedMethodNotAllowed();
protected:
	Request(RequestPrivate* dd);
private:
	Q_DECLARE_PRIVATE(Request)
protected:
	RequestPrivate* d_ptr;
};

}

#endif
