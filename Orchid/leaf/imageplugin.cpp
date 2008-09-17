#include "imageplugin.h"

#include "imageplugin.moc"

#include <stem/resourcefactory.h>
#include <leaf/imageresource.h>
#include <leaf/imagecollection.h>

using namespace Orchid;

class ImageResourceFactory : public ResourceFactoryHelper {
	QStringList keys() const {
		QStringList list;
		list << "Image";
		list << "ImageCollection";
		list << "ImageCollectionScaling";
		return list;
	}
	Resource::Base *create(const QString &key) {
		Resource::Base *res = 0;
		if(key == "Image") {
			res = new ImageResource();
		} else if(key == "ImageCollection") {
			res = new ImageCollection();
		} else if(key == "ImageCollectionScaling") {
			res = new ImageCollectionScaling();
		}
		return res;
	}
};

ImagePlugin::ImagePlugin() {
	factory = new ImageResourceFactory();
}

ImagePlugin::~ImagePlugin() {
	delete factory;
}

QList<FactoryHelper*> ImagePlugin::helpers() const {
	return QList<FactoryHelper*>() << factory;
}

Q_EXPORT_PLUGIN2(orchid_imageres_extension, ImagePlugin)
