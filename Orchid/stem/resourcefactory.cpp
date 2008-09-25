#include "resourcefactory.h"

#include "resource.h"

namespace Orchid {
	
QStringList ResourceFactory::keys() {
	QStringList list;
	list << ContainerResource::staticTypeName();
	list << ExtensionManager::keys("resource");
	return list;
}

Resource::Base *ResourceFactory::create(const QString &key) {
	Resource::Base *res = 0;
	if(key == ContainerResource::staticTypeName()) {
		res = new ContainerResource();
	}
	if(!res) {
		FactoryHelper* helper = ExtensionManager::helper("resource", key);
		ResourceFactoryHelperBase *resHelper = dynamic_cast<ResourceFactoryHelperBase*>(helper);
		if(helper)
			res = resHelper->create();
	}
	return res;
}

}
