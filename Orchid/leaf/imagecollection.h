#ifndef _ORCHID_IMAGECOLLECTION_H_
#define _ORCHID_IMAGECOLLECTION_H_

#include <stem/resource.h>
#include <stem/resourcekeep.h>

namespace Orchid {

class ImageResource;
class ImageCollectionMod;

class ImageCollectionPrivate;
class ImageCollection : public Resource::IResource, public Resource::IDirectory {
public:
	ImageCollection();
	ImageCollection(const QVector<QPair<QString,QString> > &files);
	~ImageCollection();
public:
	bool insertFile(const QString &name, const QString &path);
	bool insertModification(const QString &name, ImageCollectionMod* mod);
	QStringList childs() const;
	QStringList images() const;
	Resource::Handle child(const QString&);
	QString path(const QString &name) const;
protected:
	ImageCollection(ImageCollectionPrivate*);
	ImageCollectionPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ImageCollection)
};

class ImageCollectionModPrivate;
class ImageCollectionMod : public Resource::IResource, public Resource::IDirectory {
public:
	ImageCollectionMod();
	~ImageCollectionMod();
public:
	void setCollection(ImageCollection* collection);
	QStringList childs() const;
	Resource::Handle child(const QString&);
protected:
	virtual ImageResource* createResource(const QString& path) = 0;
protected:
	ImageCollectionMod(ImageCollectionModPrivate *d);
	ImageCollectionModPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ImageCollectionMod);
};

class ImageCollectionScalingPrivate;
class ImageCollectionScaling : public ImageCollectionMod {
public:
	ImageCollectionScaling(int width, int height);
public:
	int width() const;
	int height() const;
	void setWidth(int width);
	void setHeight(int height);
protected:
	ImageResource* createResource(const QString& path);
private:
	Q_DECLARE_PRIVATE(ImageCollectionScaling);
};

}

#endif
