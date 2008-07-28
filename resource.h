#ifndef _ORCHID_RESOURCE_H_ 
#define _ORCHID_RESOURCE_H_

#include <QString>
#include <QStringList>
#include <QUrl>

namespace Orchid {

class Builder;

namespace Resource {

class Handle;
class Resource {
public:
	virtual ~Resource() {}
public:
	static Handle locateUrl(const Handle& handle, const QUrl& url);
};

class IDirectory {
public:
	virtual QStringList childs() const = 0;
	virtual Handle child(const QString& name) const = 0;
};

class IRedirecting {
public:
	virtual Handle locate(const QUrl& url) const = 0;
};

}

class RestResource : public Resource::Resource {
public:
	virtual void methodGet(Builder* builder);
	virtual void methodPost(Builder* builder);
	virtual void methodPut(Builder* builder);
};

class SimpleTextResource : public RestResource {
public:
	SimpleTextResource(const QString& text);
public:
	void methodGet(Builder* builder);
private:
	QString m_text;
};

class AbstractDirectoryResource : public Resource::Resource {
public:
	virtual QStringList subDirs() const = 0;
};

};

#endif
