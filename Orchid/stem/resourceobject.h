#ifndef _ORCHID_RESOURCEOBJECT_H_
#define _ORCHID_RESOURCEOBJECT_H_

#include <QtCore/QObject>
#include "resource.h"

#define ORCHID_RESOURCE_OBJECT(NAME) \
	ORCHID_RESOURCE(NAME) \
	Q_OBJECT

namespace Orchid {

namespace Resource {

class Object : public QObject,
	public Base,
	public IDirectory
{
	ORCHID_RESOURCE_OBJECT("Object");
public:
	QStringList childs() const;
	Handle child(const QString& name);
protected:
	virtual void childResource(Handle &handle);
};


}

}

#endif
