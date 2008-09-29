#ifndef _ORCHID_RESOURCEOBJECT_H_
#define _ORCHID_RESOURCEOBJECT_H_

#include <QtCore/QObject>
#include "resource.h"

#define ORCHID_RESOURCE_OBJECT(NAME) \
	ORCHID_RESOURCE(NAME)

namespace Orchid {

namespace Resource {

class ORCHID_STEM_EXPORT Object : public QObject,
	public Base,
	public IDirectory
{
	ORCHID_RESOURCE_OBJECT("Object")
	Q_OBJECT
	Q_INTERFACES(Orchid::Resource::IDirectory)
public:
	QStringList childs() const;
	Handle child(const QString& name);
protected:
	void *interfaceCast(const char *name);
	virtual void childResource(Handle &handle);
};


}

}

#endif
