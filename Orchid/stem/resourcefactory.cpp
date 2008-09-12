#include "resourcefactory.h"

#include "resource.h"

// TODO remove this cross-library includes
#include <leaf/xmlmodelresource.h>
#include <leaf/imageresource.h>
#include <leaf/imagecollection.h>

namespace Orchid {

class ResourceFactoryPrivate {
public:
	ResourceFactoryPrivate();
	~ResourceFactoryPrivate();
};

Q_GLOBAL_STATIC(ResourceFactoryPrivate, resourceFactory);

QStringList ResourceFactory::keys() {
	QStringList list;
	list << "Container";
	
	// TODO move them out into plugins
	list << "Model";
	list << "XmlModel";
	list << "Image";
	list << "ImageCollection";
	return list;
}

Resource::IResource *ResourceFactory::create(const QString &key) {
	Resource::IResource *res = 0;
	if(key == "Container") {
		res = new ContainerResource();
	} else if(key == "Model") {
		res = new ModelResource();
	} else if(key == "XmlModel") {
		res = new XmlModelResource();
	} else if(key == "Image") {
		res = new ImageResource();
	} else if(key == "ImageCollection") {
		res = new ImageCollection();
	}
	return res;
}



}
