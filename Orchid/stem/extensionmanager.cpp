#include "extensionmanager.h"

#include <QtCore/QStringList>
#include <QtCore/QMap>

// TODO remove this cross-library includes
#include <leaf/xmlmodelresource.h>
#include <leaf/imageresource.h>
#include <leaf/imagecollection.h>
#include "resourcefactory.h"

namespace Orchid {

// TODO move the plugin-classes out into plugins
class ImageResourceFactory : public ResourceFactoryHelper {
	QStringList keys() const {
		QStringList list;
		list << "Image";
		list << "ImageCollection";
		return list;
	}
	Resource::IResource *create(const QString &key) {
		Resource::IResource *res = 0;
		if(key == "Image") {
			res = new ImageResource();
		} else if(key == "ImageCollection") {
			res = new ImageCollection();
		}
		return res;
	}
};
	
class ModelResourceFactory : public ResourceFactoryHelper {
	QStringList keys() const {
		QStringList list;
		list << "Model";
		list << "XmlModel";
		return list;
	}
	Resource::IResource *create(const QString &key) {
		Resource::IResource *res = 0;
		if(key == "Model") {
			res = new ModelResource();
		} else if(key == "XmlModel") {
			res = new XmlModelResource();
		}
		return res;
	}
};



class ExtensionManagerPrivate {
public:
	ExtensionManagerPrivate();
public:
	QMap<QString,FactoryHelper*> lookup;
};

ExtensionManagerPrivate::ExtensionManagerPrivate() {
	// TODO load helpers from plugins
	
	FactoryHelper* helper = new ImageResourceFactory();
	{
		QStringList keys = helper->keys();
		QStringList::iterator it;
		for(it = keys.begin(); it != keys.end(); ++it) {
			lookup.insert(*it, helper);
		}
	}
	helper = new ModelResourceFactory();
	{
		QStringList keys = helper->keys();
		QStringList::iterator it;
		for(it = keys.begin(); it != keys.end(); ++it) {
			lookup.insert(*it, helper);
		}
	}
}

Q_GLOBAL_STATIC(ExtensionManagerPrivate, extensionManager);

QStringList ExtensionManager::keys(const QString &category) {
	// TODO use category for lookups
	return extensionManager()->lookup.keys();
}

FactoryHelper *ExtensionManager::helper(const QString &category, const QString &key) {
	// TODO use category for lookups
	return extensionManager()->lookup.value(key);
}

}
