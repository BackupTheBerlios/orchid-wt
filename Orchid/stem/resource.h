#ifndef _ORCHID_RESOURCE_H_ 
#define _ORCHID_RESOURCE_H_

#include <QString>
#include <QStringList>
#include <QUrl>

namespace Orchid {

typedef int InterfaceId;
int regInterface(const char *name);

}

template<class T>
struct OrchidInterfaceIdDecl {
	enum { Defined = 0 };
};

namespace Orchid {

template<class T>
inline InterfaceId interfaceId() {
	return OrchidInterfaceIdDecl<T*>::id();
}

template<class T>
inline InterfaceId interfacePtrId() {
	return OrchidInterfaceIdDecl<T>::id();
}

class Request;

namespace Resource {

class Handle;
class IResource {
public:
	virtual ~IResource() {}
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

class IDynamic {
public:
	virtual bool provides(InterfaceId id) = 0;
};

template <class T>
inline T cast(IResource* res) {
	IDynamic* dyn = dynamic_cast<IDynamic*>(res);
	if(dyn && !dyn->provides(interfacePtrId<T>()))
		return 0;
	return dynamic_cast<T>(res);
}

}

class ContainerResourcePrivate;
class ContainerResource : public Resource::IResource, public Resource::IDirectory {
public:
	ContainerResource();
	~ContainerResource();
public:
	bool addResource(const QString& name, Resource::IResource* res);
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString& name);
protected:
	ContainerResourcePrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ContainerResource)
};

}

#define ORCHID_DECLARE_INTERFACE(type) \
template <> \
struct OrchidInterfaceIdDecl<type*> { \
	enum { Defined = 1 }; \
	static ::Orchid::InterfaceId id() { \
		static QAtomicInt id(0); \
		if(!id) \
			id = ::Orchid::regInterface(#type); \
		return id; \
	} \
};

ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IDirectory);
ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IRedirecting);
ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IQueryable);
ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IDynamic);

#endif
