/*
 * This file is part of the Orchid libraries
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _ORCHID_RESOURCE_H_ 
#define _ORCHID_RESOURCE_H_

#include <QString>
#include <QStringList>
#include <QUrl>

#include "resourceglobals.h"

#include "resourcebase.h"

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

class IDirectory {
public:
	virtual QStringList childs() const = 0;
	virtual Handle child(const QString& name) = 0;
};

class IContainer : public IDirectory {
public:
	virtual bool addResource(const QString& name, Base* res, Ownership ownership = OwnedExternal) = 0;
	virtual bool remove(const QString &name) = 0;
	virtual bool removeAll() = 0;
};

class IRedirecting {
public:
	virtual ~IRedirecting() {}
public:
	virtual Handle locate(const QUrl& url) = 0;
};

class IQueryable {
public:
	virtual ~IQueryable() {}
public:
	virtual void query(Request* request) = 0;
};

class IConfigurable {
public:
	virtual ~IConfigurable() {}
public:
	typedef QPair<QString,int> Option;
public:
	virtual QList<Option> optionList() const = 0;
	virtual QVariant option(const QString& option) const = 0;
	virtual bool setOption(const QString& option, const QVariant& value) = 0;
};

class IAdvancedConfigurable : public IConfigurable {
public:
	virtual QVariant optionProperty(const QString& option, const QString& property) const = 0;
};

template <class T>
inline T cast(Base* res) {
	// TODO change this cast to using a cast helper in Resource::Base
	return dynamic_cast<T>(res);
}

}

class ContainerResourcePrivate;
class ContainerResource : public Resource::Base, public Resource::IContainer {
	ORCHID_RESOURCE("Container")
public:
	ContainerResource();
	~ContainerResource();
public:
	bool addResource(const QString& name, Resource::Base* res, Resource::Ownership ownership = Resource::OwnedExternal);
	bool remove(const QString &name);
	bool removeAll();
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString& name);
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
ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IContainer);
ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IRedirecting);
ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IQueryable);
ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IConfigurable);
ORCHID_DECLARE_INTERFACE(::Orchid::Resource::IAdvancedConfigurable);

#endif
