#ifndef _ORCHID_RESOURCEFACTORY_H_
#define _ORCHID_RESOURCEFACTORY_H_

#include "extensionmanager.h"

#include <stem/resourcebase.h>

class QString;

namespace Orchid {

namespace Resource {
class Base;
}

class ResourceFactoryHelperBase : public FactoryHelper {
public:
	virtual Resource::Base *create() = 0;
};

template <class T>
class ResourceFactoryHelper : public ResourceFactoryHelperBase {
public:
	ResourceFactoryHelper() {
		// compiletime check for ORCHID_RESOURCE macro in T
		T *p = static_cast<T*>(0);
		p->checkForResourceMacro(p);
	}
public:
	QString key() const {
		return T::staticTypeName();
	}
	Resource::Base *create() {
		return new T();
	}
};

class ResourceFactory {
public:
	static QStringList keys();
	static Resource::Base *create(const QString &key);
};

}

#endif
