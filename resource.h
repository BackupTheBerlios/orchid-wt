#ifndef _ORCHID_RESOURCE_H_ 
#define _ORCHID_RESOURCE_H_

#include <QString>
#include <QStringList>

namespace Orchid {

class Builder;

namespace Resource {

class Resource {
public:
	virtual ~Resource() {}
};

class Handle;
class IDirectory {
public:
	virtual QStringList childs() const = 0;
	virtual Handle child(const QString& name) = 0;
};

}

class RestResource : public Resource::Resource {
public:
	virtual void get(Builder* builder);
	virtual void post(Builder* builder);
	virtual void put(Builder* builder);
};

class SimpleTextResource : public RestResource {
public:
	SimpleTextResource(const QString& text);
public:
	void get(Builder* builder);
private:
	QString m_text;
};

class AbstractDirectoryResource : public Resource::Resource {
public:
	virtual QStringList subDirs() const = 0;
};

};

#endif
