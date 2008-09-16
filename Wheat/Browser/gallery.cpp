#include "gallery.h"

#include <stem/location.h>
#include <stem/request.h>
#include <flower/htmlstreamwriter.h>
#include <QtCore/QVariant>
#include <QtXml/QXmlStreamWriter>

#include <leaf/imagecollection.h>

#include <leaf/imageresource.h>

// TODO think about validating the casts

using namespace Orchid::Resource;

class GalleryPrivate {
	Q_DECLARE_PUBLIC(Gallery)
public:
	GalleryPrivate(Gallery* gal);
protected:
	Gallery *q_ptr;
private:
	IResource *collection;
	IResource *thumbs;
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
	cast<IContainer*>(d->collection)->addResource("thumbs", d->thumbs);
}

Gallery::~Gallery() {
	Q_D(Gallery);
	delete d->collection;
	delete d_ptr;
}

void Gallery::insertFile(const QString &name, const QString &file) {
	Q_D(Gallery);
	IResource *image = new Orchid::ImageResource(file);
	cast<IConfigurable*>(image)->setOption("path", file);
	cast<IContainer*>(d->collection)->addResource(name, image);
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
	foreach(QString name, cast<IDirectory*>(d->thumbs)->childs()) {
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
	return cast<IDirectory*>(d->collection)->childs();
}

Orchid::Resource::Handle Gallery::child(const QString &name) {
	Q_D(Gallery);
	return cast<IDirectory*>(d->collection)->child(name);
}

QList<Orchid::Resource::IConfigurable::Option> Gallery::optionList() const {
	Q_D(const Gallery);
	QList<Option> list(cast<IConfigurable*>(d->collection)->optionList());
	list << Option("title", qMetaTypeId<QString>());
	list << Option("thumbnail-width", qMetaTypeId<int>());
	list << Option("thumbnail-height", qMetaTypeId<int>());
	return list;
}

QVariant Gallery::option(const QString& name) const {
	Q_D(const Gallery);
	if(name == "title") {
		return d->title;
	}
	if(name == "thumbnail-width") {
		return cast<IConfigurable*>(d->thumbs)->option("width");
	}
	if(name == "thumbnail-height") {
		return cast<IConfigurable*>(d->thumbs)->option("height");
	}
	return cast<IConfigurable*>(d->collection)->option(name);
}

bool Gallery::setOption(const QString& name, const QVariant& value) {
	Q_D(Gallery);
	bool result = false;
	if(name == "title") {
		d->title = value.toString();
		result = true;
	}
	if(name == "thumbnail-width") {
		cast<IConfigurable*>(d->thumbs)->setOption("width", value);
		result = true;
	}
	if(name == "thumbnail-height") {
		cast<IConfigurable*>(d->thumbs)->setOption("height", value);
		result = true;
	}
	if(!result) {
		result = cast<IConfigurable*>(d->collection)->setOption(name, value);
	}
	return result;
}
