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

void ImageCollectionPrivate::resetFiles() {
	Q_Q(ImageCollection);
	QStringList::iterator it;
	for(it = fileList.begin(); it != fileList.end(); ++it) {
		keep.reset(*it);
	}
	fileList.clear();
	files.clear();

	QSet<QString>::iterator mod;
	for(mod = mods.begin(); mod != mods.end(); ++mod) {
		Resource::Handle handle = keep.getHandle(*mod);
		Resource::IResource *res = handle.resource();
		ImageCollectionMod *mod = dynamic_cast<ImageCollectionMod*>(res);
		
		Q_ASSERT(mod);
		mod->setCollection(q);
	}
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
		d->fileList << it->first;
	}
}

ImageCollection::~ImageCollection() {
	delete d_ptr;
}

bool ImageCollection::addResource(const QString &name, Resource::IResource *resource, Resource::Ownership ownership) {
	bool result = false;
	
	ImageCollectionMod *mod = dynamic_cast<ImageCollectionMod*>(resource);
	if(mod) {
		result = insertModification(name, mod, ownership);
	}
	ImageResource *image;
	if(!result && (image = dynamic_cast<ImageResource*>(resource))) {
		result = insertImage(name, image, ownership);
	}
	return result;
}

bool ImageCollection::insertImage(const QString &name, ImageResource *resource, Resource::Ownership ownership) {
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
	handle.init(resource, ownership, Resource::KeepPersistant);
	d->imageList.append(name);
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
	d->fileList.append(name);
	return true;
}

bool ImageCollection::insertModification(const QString &name, ImageCollectionMod* mod, Resource::Ownership ownership) {
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
	handle.init(mod, ownership, Resource::KeepPersistant);
	mod->setCollection(this);
	d->mods.insert(name);
	return true;
}

bool ImageCollection::isImageResource(const QString &name) const {
	Q_D(const ImageCollection);
	return d->imageList.contains(name);
}

bool ImageCollection::isImageFile(const QString &name) const {
	Q_D(const ImageCollection);
	return d->fileList.contains(name);
}

bool ImageCollection::isModification(const QString &name) const {
	Q_D(const ImageCollection);
	return d->mods.contains(name);
}

QString ImageCollection::imageFilePath(const QString &name) const {
	Q_D(const ImageCollection);
	return d->files.value(name);
}

QStringList ImageCollection::imageList() const {
	Q_D(const ImageCollection);
	return d->imageList + d->fileList;
}

QStringList ImageCollection::childs() const {
	Q_D(const ImageCollection);
	QStringList list(d->mods.toList());
	list << d->imageList;
	list << d->fileList;
	return list;
}

Resource::Handle ImageCollection::child(const QString &name) {
	Q_D(ImageCollection);
	
	Resource::Handle handle = d->keep.getHandle(name);
	if(handle.isEmpty()) {
		QString path = d->files.value(name);
		if(path.isNull()) return Resource::Handle();
		handle.init(new ImageResource(path), Resource::OwnedPrivate);
	}
	return handle;
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
		d->resetFiles();
		for(it = list.begin(); it != list.end(); ++it) {
			// TODO check for mods
			int pos = it->indexOf(':');
			QString name(it->left(pos));
			d->files.insert(name, it->right(it->size()-pos-1));
			d->fileList.append(name);
		}
		result = true;
	}
	return result;
}


ImageCollectionModPrivate::ImageCollectionModPrivate(ImageCollectionMod* mod) {
	q_ptr = mod;
	collection = 0;
};

void ImageCollectionModPrivate::resetKeep() {
	keep.resetAll();
}

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
	d->resetKeep();
}

QStringList ImageCollectionMod::childs() const {
	Q_D(const ImageCollectionMod);
	if(!d->collection) return QStringList();
	return d->collection->imageList();
}

Resource::Handle ImageCollectionMod::child(const QString &name) {
	Q_D(ImageCollectionMod);
	
	Resource::Handle handle;
	if(!d->collection) return Resource::Handle();
	if(d->collection->isImageResource(name)) {
		handle = d->keep.getHandle(name);
		
		Resource::Handle source = d->collection->child(name);
		ImageResource *res = dynamic_cast<ImageResource*>(source.resource());
		handle.init(createResource(res));
	} else if(d->collection->isImageFile(name)) {
		handle = d->keep.getHandle(name);
		
		QString path = d->collection->imageFilePath(name);
		handle.init(createResource(path));
	}
	if(handle.isEmpty()) {
		return Resource::Handle();
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
			d->resetKeep();
			result = true;
		}
	} else if(option == "height") {
		int height = value.toInt();
		if(height > 0) {
			d->height = height;
			d->resetKeep();
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

ImageResource *ImageCollectionScaling::createResource(const ImageResource *other) {
	Q_D(ImageCollectionScaling);
	ImageResource* res = other->clone();
	res->setScaling(d->width, d->height);
	return res;
}

}

