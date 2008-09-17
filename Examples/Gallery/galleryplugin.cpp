#include "galleryplugin.h"

#include "galleryplugin.moc"

#include <stem/resourcefactory.h>
#include "gallerydemo.h"

using namespace Orchid;

class GalleryResourceFactory : public ResourceFactoryHelper {
	QStringList keys() const {
		QStringList list;
		list << "Gallery";
		list << "GalleryDemo";
		return list;
	}
	Resource::Base *create(const QString &key) {
		Resource::Base *res = 0;
		if(key == "Gallery") {
			res = new Gallery();
		} else if(key == "GalleryDemo") {
			res = new GalleryDemo();
		}
		return res;
	}
};

GalleryPlugin::GalleryPlugin() {
	factory = new GalleryResourceFactory();
}

GalleryPlugin::~GalleryPlugin() {
	delete factory;
}

QList<FactoryHelper*> GalleryPlugin::helpers() const {
	return QList<FactoryHelper*>() << factory;
}

Q_EXPORT_PLUGIN2(examlpes_gallery_extension, GalleryPlugin)
