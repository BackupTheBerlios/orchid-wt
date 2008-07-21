#ifndef _ORCHID_BUILDER_H_
#define _ORCHID_BUILDER_H_

#include <QtCore/qnamespace.h>

class QIODevice;
class QUrl;

namespace Orchid {

class BuilderPrivate;

class Builder {
public:
	Builder();
	virtual ~Builder();
public:
	QIODevice* device() const;
	virtual void setDevice(QIODevice* device);

	// makes a global url from an local url
	QUrl resolve(const QUrl& url);
protected:
	Builder(BuilderPrivate* dd);
private:
	Q_DECLARE_PRIVATE(Builder)
protected:
	BuilderPrivate* d_ptr;
};

}

#endif
