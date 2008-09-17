#ifndef _ORCHID_IMAGECOLLECTION_H_
#define _ORCHID_IMAGECOLLECTION_H_

#include <stem/resource.h>
#include <stem/resourcekeep.h>

namespace Orchid {

class ImageResource;
class ImageCollectionMod;

class ImageCollectionPrivate;
class ImageCollection : 
	public Resource::IResource,
	public Resource::IContainer,
	public Resource::IConfigurable
{
public:
	ImageCollection();
	ImageCollection(const QVector<QPair<QString,QString> > &files);
	~ImageCollection();
public:
	bool addResource(const QString &name, Resource::IResource *resoure, Resource::Ownership ownership);
	bool insertImage(const QString &naem, ImageResource *resource, Resource::Ownership ownership);
	bool insertFile(const QString &name, const QString &path);
	bool insertModification(const QString &name, ImageCollectionMod* mod, Resource::Ownership ownership);

	bool isImageResource(const QString &name) const;
	bool isImageFile(const QString &name) const;
	bool isModification(const QString &name) const;
	QString imageFilePath(const QString &name) const;
	QStringList imageList() const;
	QStringList childs() const;
	Resource::Handle child(const QString&);
	QList<Option> optionList() const;
	QVariant option(const QString &option) const;
	bool setOption(const QString &option, const QVariant &value);
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
	virtual ImageResource* createResource(const ImageResource *other) = 0;
protected:
	ImageCollectionMod(ImageCollectionModPrivate *d);
	ImageCollectionModPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ImageCollectionMod);
};

class ImageCollectionScalingPrivate;
class ImageCollectionScaling : public ImageCollectionMod, public Resource::IConfigurable
{
public:
	ImageCollectionScaling();
	ImageCollectionScaling(int width, int height);
public:
	int width() const;
	int height() const;
	void setWidth(int width);
	void setHeight(int height);
	QList<Option> optionList() const;
	QVariant option(const QString &option) const;
	bool setOption(const QString &option, const QVariant &value);
protected:
	ImageResource* createResource(const QString& path);
	ImageResource* createResource(const ImageResource *other);
private:
	Q_DECLARE_PRIVATE(ImageCollectionScaling);
};

}

#endif
