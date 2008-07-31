#ifndef _ORCHID_RESOURCE_H_ 
#define _ORCHID_RESOURCE_H_

#include <QString>
#include <QStringList>
#include <QUrl>

namespace Orchid {

class Request;

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

class IQueryable {
public:
	virtual void query(Request* request) = 0;
};

}

class RestResource : public Resource::Resource {
public:
	virtual void methodGet(Request* request);
	virtual void methodPost(Request* request);
	virtual void methodPut(Request* request);
};

class SimpleTextResource : public RestResource, public Resource::IQueryable {
public:
	SimpleTextResource(const QString& text);
public:
	void methodGet(Request* request);
	void query(Request* request);
private:
	QString m_text;
};

class AbstractDirectoryResource : public Resource::Resource {
public:
	virtual QStringList subDirs() const = 0;
};

};

#endif
