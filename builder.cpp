#include "builder.h" 
#include "obuilder_p.h"

#include <QtXml/QXmlStreamWriter>

namespace Bamboo {

class BuilderPrivate : public Orchid::BuilderPrivate {
	Q_DECLARE_PUBLIC(Builder)
public:
    BuilderPrivate(Builder *builder)
        : Orchid::BuilderPrivate(builder), q_ptr(builder)
    { }
private:
    Builder* q_ptr;
	QIODevice* device;
	QXmlStreamWriter xml;
};

Builder::Builder() {
    d_ptr = new BuilderPrivate(this);
}

QXmlStreamWriter* Builder::xml() {
	Q_D(Builder);
	return &d->xml;
}

void Builder::setDevice(QIODevice* device) {
	Orchid::Builder::setDevice(device);

	Q_D(Builder);
	d->xml.setDevice(device);
}

}
