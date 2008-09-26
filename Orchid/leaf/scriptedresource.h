#ifndef _ORCHID_SCRIPTEDRESOURCE_H_
#define _ORCHID_SCRIPTEDRESOURCE_H_

#include <QObject>
#include <stem/resource.h>

namespace Orchid {

class ScriptedResourcePrivate;
class ScriptedResource :
	public QObject,
	public Resource::Base,
	public Resource::IQueryable,
	public Resource::IDirectory,
	public Resource::IDynamic
{
	Q_OBJECT
public:
	ScriptedResource(const QString& script, const QString& type);
	~ScriptedResource();
public:
	void query(Orchid::Request*);
	bool provides(InterfaceId id);
	QStringList childs() const;
	Resource::Handle child(const QString& name);
protected:
	ScriptedResourcePrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ScriptedResource)
};

}

#endif
