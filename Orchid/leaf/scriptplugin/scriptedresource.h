#ifndef _ORCHID_SCRIPTEDRESOURCE_H_
#define _ORCHID_SCRIPTEDRESOURCE_H_

#include <QObject>
#include <stem/resourceobject.h>

namespace Orchid {

class ScriptedResourcePrivate;
class ScriptedResource :
	public Resource::Object,
	public Resource::IQueryable,
	public Resource::IConfigurable
{
	ORCHID_RESOURCE_OBJECT("Scripted-Resource")
	Q_OBJECT
	Q_INTERFACES(Orchid::Resource::IQueryable
	             Orchid::Resource::IConfigurable)
public:
	ScriptedResource();
	ScriptedResource(const QString &path);
	~ScriptedResource();
public:
	bool loadScript(const QString &path);
	void query(Orchid::Request*);
	QStringList childs() const;
	Resource::Handle child(const QString& name);
	QList<Option> optionList() const;
	QVariant option(const QString&) const;
	bool setOption(const QString&, const QVariant&);
protected:
	ScriptedResourcePrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ScriptedResource)
};

}

#endif
