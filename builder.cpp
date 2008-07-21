#include "builder.h" 
#include "obuilder_p.h"

#include <QtXml/QXmlStreamWriter>
#include <QHash>

#include "styleattributes.h"

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
	QHash<const Style*, StyleAttributes> styles;
};

Builder::Builder() : Orchid::Builder(new BuilderPrivate(this)) {
}

Builder::Builder(QIODevice* device) : Orchid::Builder(new BuilderPrivate(this)) {
	Q_D(Builder);
	
	d->device = device;
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

StyleAttributes Builder::attributes(const Style* style) {
	Q_D(Builder);
	return d->styles.value(style);
}

void Builder::regStyle(const Style* style, const QString& prefix) {
	Q_D(Builder);
	d->styles.insert(style, StyleAttributes(prefix));
}

}
