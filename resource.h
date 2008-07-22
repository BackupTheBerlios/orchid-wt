#ifndef _ORCHID_RESOURCE_H_ 
#define _ORCHID_RESOURCE_H_

#include <QString>
#include <QStringList>

namespace Orchid {

class Builder;

class Resource {
public:
	virtual ~Resource() {}
// public:
// 	QString url() const;
// private:
// 	QString m_url;
};

class RestResource : public Resource {
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

class AbstractDirectoryResource : public Resource {
public:
	virtual QStringList subDirs() const = 0;
};

};

#endif
