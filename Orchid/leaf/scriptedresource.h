#ifndef _ORCHID_SCRIPTEDRESOURCE_H_
#define _ORCHID_SCRIPTEDRESOURCE_H_

#include <QObject>
#include <stem/resourceobject.h>

namespace Orchid {

class ScriptedResourcePrivate;
class ScriptedResource :
	public Resource::Object,
	public Resource::IQueryable
{
	ORCHID_RESOURCE_OBJECT("Scripted-Resource")
	Q_OBJECT
	Q_INTERFACES(Orchid::Resource::IQueryable)
public:
	ScriptedResource(const QString& script, const QString& type);
	~ScriptedResource();
public:
	void query(Orchid::Request*);
	QStringList childs() const;
	Resource::Handle child(const QString& name);
protected:
	ScriptedResourcePrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ScriptedResource)
};

}

#endif
