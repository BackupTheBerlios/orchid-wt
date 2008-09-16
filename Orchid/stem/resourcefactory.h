#ifndef _ORCHID_RESOURCEFACTORY_H_
#define _ORCHID_RESOURCEFACTORY_H_

#include "extensionmanager.h"

class QString;
class QStringList;

namespace Orchid {

namespace Resource {
class IResource;
}

class ResourceFactoryHelper : public FactoryHelper {
public:
	virtual Resource::IResource *create(const QString &key) = 0;
};

class ResourceFactory {
public:
	static QStringList keys();
	static Resource::IResource *create(const QString &key);
};

}

#endif
