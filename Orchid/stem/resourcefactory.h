#ifndef _ORCHID_RESOURCEFACTORY_H_
#define _ORCHID_RESOURCEFACTORY_H_

class QString;
class QStringList;

namespace Orchid {

namespace Resource {
class IResource;
}

class ResourceFactory {
public:
	static QStringList keys();
	static Resource::IResource *create(const QString &key);
};

}

#endif
