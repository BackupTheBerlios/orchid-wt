#include "resourcefactory.h"

#include "resource.h"

namespace Orchid {
	
QStringList ResourceFactory::keys() {
	QStringList list;
	list << ExtensionManager::keys("resource");
	list << "Container";
	return list;
}

Resource::IResource *ResourceFactory::create(const QString &key) {
	Resource::IResource *res = 0;
	if(key == "Container") {
		res = new ContainerResource();
	}
	if(!res) {
		FactoryHelper* helper = ExtensionManager::helper("resource", key);
		ResourceFactoryHelper *resHelper = dynamic_cast<ResourceFactoryHelper*>(helper);
		if(helper)
			res = resHelper->create(key);
	}
	return res;
}

}
