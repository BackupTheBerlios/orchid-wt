#include "gallery.h"

#include <stem/location.h>
#include <stem/request.h>
#include <flower/htmlstreamwriter.h>
#include <QtCore/QVariant>
#include <QtXml/QXmlStreamWriter>

#include <leaf/imagecollection_p.h>

class GalleryPrivate : public Orchid::ImageCollectionPrivate {
	Q_DECLARE_PUBLIC(Gallery)
public:
	GalleryPrivate(Gallery* gal);
private:
	Orchid::ImageCollectionScaling *thumbs;
	QString title;
};

GalleryPrivate::GalleryPrivate(Gallery* gal)
	: Orchid::ImageCollectionPrivate(gal)
{
	title = "Gallery";
	thumbs = 0;
}

Gallery::Gallery() : ImageCollection(new GalleryPrivate(this)) {
	Q_D(Gallery);
	d->thumbs = new Orchid::ImageCollectionScaling(160, 160);
	insertModification("thumbs", d->thumbs);
}

void Gallery::query(Orchid::Request *request) {
	Q_D(Gallery);
	if(!(request->method() & Orchid::GetMethod)) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;
	
	Orchid::XHtml11StreamWriter writer;
	writer.setDevice(request);
	writer.xmlWriter()->setAutoFormatting(true);
	
	writer.writeStartDocument();
	writer.writeStartElement(Orchid::HtmlTagHeading);
	writer.writeCharacters(d->title);
	writer.writeEndElement();
	foreach(QString name, images()) {
		writer.xmlWriter()->writeStartElement("a");
		writer.xmlWriter()->writeAttribute("href", request->url(request->location().relative(name)));
		writer.xmlWriter()->writeEmptyElement("img");
		writer.xmlWriter()->writeAttribute("src", request->url(request->location().relative("thumbs/"+name)));
		writer.xmlWriter()->writeEndElement();
	}
	writer.writeEndDocument();
}

QList<Orchid::Resource::IConfigurable::Option> Gallery::optionList() const {
	QList<Option> list;
	list << Option("urls", qMetaTypeId<QStringList>());
	list << Option("title", qMetaTypeId<QString>());
	list << Option("thumbnail-width", qMetaTypeId<int>());
	list << Option("thumbnail-height", qMetaTypeId<int>());
	return list;
}

QVariant Gallery::option(const QString& name) const {
	Q_D(const Gallery);
	if(name == "urls") {
	}
	if(name == "title") {
		return d->title;
	}
	if(name == "thumbnail-width") {
		return d->thumbs->width();
	}
	if(name == "thumbnail-height") {
		return d->thumbs->height();
	}
	return QVariant();
}

bool Gallery::setOption(const QString& name, const QVariant& value) {
	Q_D(Gallery);
	if(name == "urls") {
	}
	if(name == "title") {
		d->title = value.toString();
	}
	if(name == "thumbnail-width") {
		d->thumbs->setWidth(value.toInt());
	}
	if(name == "thumbnail-height") {
		d->thumbs->setHeight(value.toInt());
	}
	return false;
}
