#ifndef _ORCHID_IMAGECOLLECTION_P_H_
#define _ORCHID_IMAGECOLLECTION_P_H_

#include <QtCore/QHash>
#include <QtCore/QSet>

namespace Orchid {

class ImageCollection;
class ImageCollectionMod;
class ImageCollectionScaling;

class ImageCollectionPrivate {
	Q_DECLARE_PUBLIC(ImageCollection)
public:
	ImageCollectionPrivate(ImageCollection* collection);
protected:
	ImageCollection* q_ptr;
private:
	QStringList namelist;
	QHash<QString,QString> files;
	QSet<QString> mods;
	Orchid::Resource::Keep keep;
};

class ImageCollectionModPrivate {
	Q_DECLARE_PUBLIC(ImageCollectionMod)
public:
	ImageCollectionModPrivate(ImageCollectionMod* mod);
protected:
	ImageCollectionMod* q_ptr;
private:
	ImageCollection* collection;
	Resource::Keep keep;
};

class ImageCollectionScalingPrivate : public ImageCollectionModPrivate {
public:
	ImageCollectionScalingPrivate(ImageCollectionScaling* scaling);
private:
	Q_DECLARE_PUBLIC(ImageCollectionScaling)
	int width, height;
};

}

#endif
