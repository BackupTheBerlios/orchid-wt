#include "gallery.h"

#include <stem/location.h>
#include <stem/request.h>
#include <flower/htmlstreamwriter.h>
#include <QtCore/QVariant>
#include <QtXml/QXmlStreamWriter>

#include <leaf/imagecollection.h>

class GalleryPrivate {
	Q_DECLARE_PUBLIC(Gallery)
public:
	GalleryPrivate(Gallery* gal);
protected:
	Gallery *q_ptr;
private:
	Orchid::ImageCollection *collection;
	Orchid::ImageCollectionScaling *thumbs;
	QString title;
};

GalleryPrivate::GalleryPrivate(Gallery* gal) {
	q_ptr = gal;
	title = "Gallery";
	collection = 0;
	thumbs = 0;
}

Gallery::Gallery() {
	d_ptr = new GalleryPrivate(this);
	Q_D(Gallery);
	d->thumbs = new Orchid::ImageCollectionScaling(160, 160);
	d->collection = new Orchid::ImageCollection();
	d->collection->insertModification("thumbs", d->thumbs);
}

Gallery::~Gallery() {
	Q_D(Gallery);
	delete d->collection;
	delete d_ptr;
}

void Gallery::insertFile(const QString &name, const QString &file) {
	Q_D(Gallery);
	d->collection->insertFile(name, file);
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
	foreach(QString name, d->collection->images()) {
		writer.xmlWriter()->writeStartElement("a");
		writer.xmlWriter()->writeAttribute("href", request->url(request->location().relative(name)));
		writer.xmlWriter()->writeEmptyElement("img");
		writer.xmlWriter()->writeAttribute("src", request->url(request->location().relative("thumbs/"+name)));
		writer.xmlWriter()->writeEndElement();
	}
	writer.writeEndDocument();
}

QStringList Gallery::childs() const {
	Q_D(const Gallery);
	return d->collection->childs();
}

Orchid::Resource::Handle Gallery::child(const QString &name) {
	Q_D(Gallery);
	return d->collection->child(name);
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
	bool result = false;
	if(name == "urls") {
	}
	if(name == "title") {
		d->title = value.toString();
		result = true;
	}
	if(name == "thumbnail-width") {
		d->thumbs->setWidth(value.toInt());
		result = true;
	}
	if(name == "thumbnail-height") {
		d->thumbs->setHeight(value.toInt());
		result = true;
	}
	return result;
}
