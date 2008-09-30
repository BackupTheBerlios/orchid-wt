/*
 * This file is part of the Orchid example plugins
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "gallery.h"

#include <stem/location.h>
#include <stem/request.h>
#include <stem/resourcefactory.h>
#include <stem/resourcekeep.h>
#include <flower/xhtmlstreamwriter.h>
#include <QtCore/QVariant>
#include <QtXml/QXmlStreamWriter>

using namespace Orchid;
using namespace Orchid::Resource;

class GalleryPrivate {
	Q_DECLARE_PUBLIC(Gallery)
public:
	GalleryPrivate(Gallery* gal);
protected:
	Gallery *q_ptr;
private:
	Base *collection;
	Base *thumbs;
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
	d->thumbs = ResourceFactory::create("ImageCollectionScaling");
	Q_ASSERT(d->thumbs);
	d->collection = ResourceFactory::create("ImageCollection");
	Q_ASSERT(d->collection);
	
	IContainer *collection = cast<IContainer*>(d->collection);
	Q_ASSERT(collection);
	collection->addResource("thumbs", d->thumbs, OwnedPrivate);
}

Gallery::~Gallery() {
	Q_D(Gallery);
	delete d->collection;
	delete d_ptr;
}

void Gallery::setTitle(const QString &title) {
	Q_D(Gallery);
	d->title = title;
}

void Gallery::insertFile(const QString &name, const QString &file) {
	Q_D(Gallery);
	Base *image = ResourceFactory::create("Image");
	IConfigurable *imageSettings = cast<IConfigurable*>(image);
	Q_ASSERT(imageSettings);
	imageSettings->setOption("path", file);
	IContainer *collection = cast<IContainer*>(d->collection);
	Q_ASSERT(collection);
	collection->addResource(name, image);
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
	
	writer.startDocument();
	writer.startElement(Orchid::Document::TagHeading);
	writer.insertCharacters(d->title);
	writer.endElement();
	IDirectory *thumbs = cast<IDirectory*>(d->thumbs);
	Q_ASSERT(thumbs);
	foreach(QString name, thumbs->childs()) {
		writer.xmlWriter()->writeStartElement("a");
		writer.xmlWriter()->writeAttribute("href", request->url(request->location().relative(name)));
		writer.xmlWriter()->writeEmptyElement("img");
		writer.xmlWriter()->writeAttribute("src", request->url(request->location().relative("thumbs/"+name)));
		writer.xmlWriter()->writeEndElement();
	}
	writer.endDocument();
}

QStringList Gallery::childs() const {
	Q_D(const Gallery);
	IDirectory *collection = cast<IDirectory*>(d->collection);
	Q_ASSERT(collection);
	return collection->childs();
}

Orchid::Resource::Handle Gallery::child(const QString &name) {
	Q_D(Gallery);
	IDirectory *collection = cast<IDirectory*>(d->collection);
	Q_ASSERT(collection);
	return collection->child(name);
}

QList<Orchid::Resource::IConfigurable::Option> Gallery::optionList() const {
	Q_D(const Gallery);
	IConfigurable *collection = cast<IConfigurable*>(d->collection);
	Q_ASSERT(collection);
	QList<Option> list(collection->optionList());
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
		IConfigurable *thumbs = cast<IConfigurable*>(d->thumbs);
		Q_ASSERT(thumbs);
		return thumbs->option("width");
	}
	if(name == "thumbnail-height") {
		IConfigurable *thumbs = cast<IConfigurable*>(d->thumbs);
		Q_ASSERT(thumbs);
		return thumbs->option("height");
	}
	IConfigurable *collection = cast<IConfigurable*>(d->collection);
	Q_ASSERT(collection);
	return collection->option(name);
}

bool Gallery::setOption(const QString& name, const QVariant& value) {
	Q_D(Gallery);
	bool result = false;
	if(name == "title") {
		d->title = value.toString();
		result = true;
	}
	if(name == "thumbnail-width") {
		IConfigurable *thumbs = cast<IConfigurable*>(d->thumbs);
		Q_ASSERT(thumbs);
		thumbs->setOption("width", value);
		result = true;
	}
	if(name == "thumbnail-height") {
		IConfigurable *thumbs = cast<IConfigurable*>(d->thumbs);
		Q_ASSERT(thumbs);
		thumbs->setOption("height", value);
		result = true;
	}
	if(!result) {
		IConfigurable *collection = cast<IConfigurable*>(d->collection);
		Q_ASSERT(collection);
		result = collection->setOption(name, value);
	}
	return result;
}
