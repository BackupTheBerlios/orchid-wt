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
	virtual Handle child(const QString& name) = 0;
};

class IRedirecting {
public:
	virtual Handle locate(const QUrl& url) = 0;
};

class IQueryable {
public:
	virtual void query(Request* request) = 0;
};

}

class ContainerResourcePrivate;
class ContainerResource : public Resource::Resource, public Resource::IDirectory {
public:
	ContainerResource();
	~ContainerResource();
public:
	bool addResource(const QString& name, Resource::Resource* res);
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString& name);
protected:
	ContainerResourcePrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ContainerResource)
};

class SimpleTextResource : public ::Orchid::Resource::Resource, public ::Orchid::Resource::IQueryable {
public:
	SimpleTextResource(const QString& text);
public:
	void query(Request* request);
private:
	QString m_text;
};

};

#endif
