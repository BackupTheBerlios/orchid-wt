#include "imagecollection.h"

#include "imagecollection_p.h"

#include <stem/location.h>
#include "imageresource.h"
#include <QtCore/QHash>
#include <QtCore/QtDebug>
#include <QtCore/QVariant>

namespace Orchid {

ImageCollectionPrivate::ImageCollectionPrivate(ImageCollection* collection) {
	q_ptr = collection;
}

ImageCollection::ImageCollection() {
	d_ptr = new ImageCollectionPrivate(this);
}

ImageCollection::ImageCollection(ImageCollectionPrivate* d) {
	d_ptr = d;
};

ImageCollection::ImageCollection(const QVector<QPair<QString,QString> >& files) {
	d_ptr = new ImageCollectionPrivate(this);
	Q_D(ImageCollection);
	QVector<QPair<QString,QString> >::const_iterator it;
	for(it = files.constBegin(); it!= files.constEnd(); ++it) {
		d->files.insert(it->first, it->second);
		d->namelist << it->first;
	}
}

ImageCollection::~ImageCollection() {
	delete d_ptr;
}

bool ImageCollection::addResource(const QString &name, Resource::IResource *resource) {
	bool result = false;
	
	ImageCollectionMod *mod = dynamic_cast<ImageCollectionMod*>(resource);
	if(mod) {
		result = insertModification(name, mod);
	}
	ImageResource *image;
	if(!result && (image = dynamic_cast<ImageResource*>(resource))) {
		result = insertImage(name, image);
	}
	return result;
}

bool ImageCollection::insertImage(const QString &name, ImageResource *resource) {
	Q_D(ImageCollection);
	QString path = resource->path();
	if(path.isNull()) {
		qWarning() << "ImageCollection requires its images to have paths";
		return false;
	}
	if(d->files.contains(name)) {
		qWarning() << "ImageCollection allready contains the file " << name;
		return false;
	}
	if(d->mods.contains(name)) {
		qWarning() << "ImageCollection allready contains the modification" << name;
		return false;
	}
	
	Resource::Handle handle = d->keep.getHandle(name);
	handle.init(resource);
	d->files.insert(name, path);
	d->namelist.append(name);
	return true;
}

bool ImageCollection::insertFile(const QString &name, const QString &path) {
	Q_D(ImageCollection);
	if(d->files.contains(name)) {
		qWarning() << "ImageCollection allready contains the file " << name;
		return false;
	}
	if(d->mods.contains(name)) {
		qWarning() << "ImageCollection allready contains the modification" << name;
		return false;
	}
	
	d->files.insert(name, path);
	d->namelist.append(name);
	return true;
}

bool ImageCollection::insertModification(const QString &name, ImageCollectionMod* mod) {
	Q_D(ImageCollection);
	if(d->files.contains(name)) {
		qWarning() << "ImageCollection allready contains the file " << name;
		return false;
	}
	if(d->mods.contains(name)) {
		qWarning() << "ImageCollection allready contains the modification" << name;
		return false;
	}
	
	Resource::Handle handle = d->keep.getHandle(name);
	handle.init(mod, Resource::KeepPersistant);
	mod->setCollection(this);
	d->mods.insert(name);
	return true;
}

QStringList ImageCollection::childs() const {
	Q_D(const ImageCollection);
	QStringList list(d->mods.toList());
	list << d->files.keys();
	return list;
}

QStringList ImageCollection::images() const {
	Q_D(const ImageCollection);
	return d->namelist;
}

Resource::Handle ImageCollection::child(const QString &name) {
	Q_D(ImageCollection);
	
	Resource::Handle handle = d->keep.getHandle(name);
	if(handle.isEmpty()) {
		QString path = d->files.value(name);
		if(path.isNull()) return Resource::Handle();
		handle.init(new ImageResource(path));
	}
	return handle;
}

QString ImageCollection::path(const QString &name) const {
	Q_D(const ImageCollection);
	return d->files.value(name);
}

QList<Resource::IConfigurable::Option> ImageCollection::optionList() const {
	QList<Option> optionList;
	optionList << Option("urls", qMetaTypeId<QStringList>());
	return optionList;
}

QVariant ImageCollection::option(const QString &option) const {
	Q_D(const ImageCollection);
	QVariant result;
	if(option == "urls") {
		QStringList list;
		QHash<QString,QString>::const_iterator it;
		for(it = d->files.begin(); it != d->files.end(); ++it) {
			list << it.key() + ':' + it.value();
		}
		result.setValue(list);
	}
	return result;
}

bool ImageCollection::setOption(const QString &option, const QVariant &value) {
	Q_D(ImageCollection);
	bool result = false;
	if(option == "urls") {
		QStringList list = value.toStringList();
		QStringList::iterator it;
		d->files.clear();
		d->namelist.clear();
		for(it = list.begin(); it != list.end(); ++it) {
			// TODO check for mods
			int pos = it->indexOf(':');
			QString name(it->left(pos));
			d->files.insert(name, it->right(it->size()-pos-1));
			d->namelist.append(name);
		}
		result = true;
	}
	return result;
}


ImageCollectionModPrivate::ImageCollectionModPrivate(ImageCollectionMod* mod) {
	q_ptr = mod;
	collection = 0;
};

ImageCollectionMod::ImageCollectionMod() {
	d_ptr = new ImageCollectionModPrivate(this);
}

ImageCollectionMod::ImageCollectionMod(ImageCollectionModPrivate* d) {
	d_ptr = d;
};

ImageCollectionMod::~ImageCollectionMod() {
	delete d_ptr;
}

void ImageCollectionMod::setCollection(ImageCollection* collection) {
	Q_D(ImageCollectionMod);
	d->collection = collection;
}

QStringList ImageCollectionMod::childs() const {
	Q_D(const ImageCollectionMod);
	if(!d->collection) return QStringList();
	return d->collection->images();
}

Resource::Handle ImageCollectionMod::child(const QString &name) {
	Q_D(ImageCollectionMod);
	
	if(!d->collection) return Resource::Handle();
	QString path = d->collection->path(name);
	if(path.isNull()) return Resource::Handle();
	
	Resource::Handle handle = d->keep.getHandle(name);
	if(handle.isEmpty()) {
		handle.init(createResource(path));
	}
	return handle;
}

ImageCollectionScalingPrivate::ImageCollectionScalingPrivate(
	ImageCollectionScaling* scaling)
	: ImageCollectionModPrivate(scaling)
{
	width = 100;
	height = 100;
}

ImageCollectionScaling::ImageCollectionScaling()
	: ImageCollectionMod(new ImageCollectionScalingPrivate(this))
{ }

ImageCollectionScaling::ImageCollectionScaling(int width, int height)
	: ImageCollectionMod(new ImageCollectionScalingPrivate(this))
{
	Q_D(ImageCollectionScaling);
	d->width = width;
	d->height = height;
}

int ImageCollectionScaling::width() const {
	Q_D(const ImageCollectionScaling);
	return d->width;
}

int ImageCollectionScaling::height() const {
	Q_D(const ImageCollectionScaling);
	return d->height;
}

void ImageCollectionScaling::setWidth(int width) {
	Q_D(ImageCollectionScaling);
	d->width = width;
}

void ImageCollectionScaling::setHeight(int height) {
	Q_D(ImageCollectionScaling);
	d->height = height;
}

QList<Resource::IConfigurable::Option> ImageCollectionScaling::optionList() const {
	QList<Option> optionList;
	optionList << Option("width", qMetaTypeId<int>());
	optionList << Option("height", qMetaTypeId<int>());
	return optionList;
}

QVariant ImageCollectionScaling::option(const QString &option) const {
	Q_D(const ImageCollectionScaling);
	QVariant result;
	if(option == "width") {
		result = d->width;
	} else if(option == "height") {
		result = d->height;
	}
	return result;
}

bool ImageCollectionScaling::setOption(const QString &option, const QVariant &value) {
	Q_D(ImageCollectionScaling);
	bool result = false;
	if(option == "width") {
		int width = value.toInt();
		if(width > 0) {
			d->width = width;
			result = true;
		}
	} else if(option == "height") {
		int height = value.toInt();
		if(height > 0) {
			d->height = height;
			result = true;
		}
	}
	return result;
}

ImageResource* ImageCollectionScaling::createResource(const QString &path) {
	Q_D(ImageCollectionScaling);
	ImageResource* res = new ImageResource(path);
	res->setScaling(d->width, d->height);
	return res;
}

}

