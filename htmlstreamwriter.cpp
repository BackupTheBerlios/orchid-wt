#include "htmlstreamwriter.h" 

#include <QtXml/QXmlStreamWriter>
#include <QHash>

#include "styleattributes.h"

namespace Bamboo {

class HtmlStreamWriterPrivate {
	Q_DECLARE_PUBLIC(HtmlStreamWriter)
public:
    HtmlStreamWriterPrivate(HtmlStreamWriter *writer) : q_ptr(writer) {
		device = 0;
	}
private:
    HtmlStreamWriter* q_ptr;
	QIODevice* device;
	QXmlStreamWriter xml;
	QHash<const Style*, StyleAttributes> styles;
};

HtmlStreamWriter::HtmlStreamWriter()
	: d_ptr(new HtmlStreamWriterPrivate(this))
{
}

HtmlStreamWriter::HtmlStreamWriter(QIODevice* device)
	: d_ptr(new HtmlStreamWriterPrivate(this))
{
	Q_D(HtmlStreamWriter);
	
	d->device = device;
	d->xml.setDevice(device);
}

QXmlStreamWriter* HtmlStreamWriter::xmlWriter() {
	Q_D(HtmlStreamWriter);
	return &d->xml;
}

void HtmlStreamWriter::setDevice(QIODevice* device) {
	Q_D(HtmlStreamWriter);
	d->device = device;
	d->xml.setDevice(device);
}

StyleAttributes HtmlStreamWriter::attributes(const Style* style) {
	Q_D(HtmlStreamWriter);
	return d->styles.value(style);
}

void HtmlStreamWriter::regStyle(const Style* style, const QString& prefix) {
	Q_D(HtmlStreamWriter);
	d->styles.insert(style, StyleAttributes(prefix));
}

}
