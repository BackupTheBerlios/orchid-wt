#include "obuilder.h"
#include "obuilder_p.h" 

#include <QtCore/QUrl>

namespace Orchid {

class UrlResolver {
public:
	QUrl resolve(const QUrl& local);
};

QUrl UrlResolver::resolve(const QUrl& local) {
	return local;
}

Builder::Builder() {
    d_ptr = new BuilderPrivate(this);
}

Builder::~Builder() {
}

QIODevice* Builder::device() const {
	Q_D(const Builder);
	return d->device;
}

void Builder::setDevice(QIODevice* device) {
	Q_D(Builder);
	d->device = device;
}

}
