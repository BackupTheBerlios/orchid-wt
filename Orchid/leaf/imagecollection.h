#ifndef _ORCHID_IMAGECOLLECTION_H_
#define _ORCHID_IMAGECOLLECTION_H_

#include <stem/resource.h>
#include <stem/resourcekeep.h>

namespace Orchid {

class ImageResource;
class ImageCollectionMod;

class ImageCollectionPrivate;
class ImageCollection : 
	public Resource::Base,
	public Resource::IContainer,
	public Resource::IConfigurable
{
	ORCHID_RESOURCE("ImageCollection")
public:
	ImageCollection();
	ImageCollection(const QVector<QPair<QString,QString> > &files);
	~ImageCollection();
public:
	bool addResource(const QString &name, Resource::Base *resoure, Resource::Ownership ownership);
	bool remove(const QString &name);
	bool removeAll();
	bool insertImage(const QString &name, ImageResource *resource, Resource::Ownership ownership);
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
private:
	Q_DECLARE_PRIVATE(ImageCollection)
};

class ImageCollectionModPrivate;
class ImageCollectionMod : public Resource::Base, public Resource::IDirectory {
	ORCHID_RESOURCE("ImageCollectionMod-unused")
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
private:
	Q_DECLARE_PRIVATE(ImageCollectionMod);
};

class ImageCollectionScalingPrivate;
class ImageCollectionScaling : public ImageCollectionMod, public Resource::IConfigurable
{
	ORCHID_RESOURCE("ImageCollectionScaling")
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
