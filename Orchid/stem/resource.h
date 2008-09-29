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

template<typename T>
const char *orchid_interface_name() { return 0.0; }

namespace Orchid {

class Request;

namespace Resource {

class Handle;

class ORCHID_STEM_EXPORT IDirectory {
public:
	virtual QStringList childs() const = 0;
	virtual Handle child(const QString& name) = 0;
};

class ORCHID_STEM_EXPORT IContainer : public IDirectory {
public:
	virtual bool addResource(const QString& name, Base* res, Ownership ownership = OwnedExternal) = 0;
	virtual bool remove(const QString &name) = 0;
	virtual bool removeAll() = 0;
};

class ORCHID_STEM_EXPORT IRedirecting {
public:
	virtual ~IRedirecting() {}
public:
	virtual Handle locate(const QUrl& url) = 0;
};

class ORCHID_STEM_EXPORT IQueryable {
public:
	virtual ~IQueryable() {}
public:
	virtual void query(Request* request) = 0;
};

class ORCHID_STEM_EXPORT IConfigurable {
public:
	virtual ~IConfigurable() {}
public:
	typedef QPair<QString,int> Option;
public:
	virtual QList<Option> optionList() const = 0;
	virtual QVariant option(const QString& option) const = 0;
	virtual bool setOption(const QString& option, const QVariant& value) = 0;
};

class ORCHID_STEM_EXPORT IAdvancedConfigurable : public IConfigurable {
public:
	virtual QVariant optionProperty(const QString& option, const QString& property) const = 0;
};

inline void *orchid_interface_cast(Base *res, const char *name) {
	return res ? res->interfaceCast(name) : 0;
}

template <class T>
inline T cast(Base* res) {
	return reinterpret_cast<T>(orchid_interface_cast(res, orchid_interface_name<T>()));
}

}

class ContainerResourcePrivate;
class ORCHID_STEM_EXPORT ContainerResource : public Resource::Base, public Resource::IContainer {
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
protected:
	void *interfaceCast(const char *name);
private:
	Q_DECLARE_PRIVATE(ContainerResource)
};

}

#define ORCHID_DECLARE_INTERFACE(TYPE, NAME) \
template <> inline const char *orchid_interface_name<TYPE*>() { return NAME; }

#define ORCHID_PROVIDE_CAST(STR, TYPE) \
	if(strcmp(STR, orchid_interface_name<TYPE>()) == 0) \
		return static_cast<TYPE>(this);


ORCHID_DECLARE_INTERFACE( Orchid::Resource::IDirectory,
	"org.orchid-wt.Orchid.ResourceInterfaces/Directory")
Q_DECLARE_INTERFACE(Orchid::Resource::IDirectory,
	"org.orchid-wt.Orchid.ResourceInterfaces/Directory")
ORCHID_DECLARE_INTERFACE(Orchid::Resource::IContainer,
	"org.orchid-wt.Orchid.ResourceInterfaces/Container")
Q_DECLARE_INTERFACE(Orchid::Resource::IContainer,
	"org.orchid-wt.Orchid.ResourceInterfaces/Container")
ORCHID_DECLARE_INTERFACE(Orchid::Resource::IRedirecting,
	"org.orchid-wt.Orchid.ResourceInterfaces/Redirecting")
Q_DECLARE_INTERFACE(Orchid::Resource::IRedirecting,
	"org.orchid-wt.Orchid.ResourceInterfaces/Redirecting")
ORCHID_DECLARE_INTERFACE(Orchid::Resource::IQueryable,
	"org.orchid-wt.Orchid.ResourceInterfaces/Queryable")
Q_DECLARE_INTERFACE(Orchid::Resource::IQueryable,
	"org.orchid-wt.Orchid.ResourceInterfaces/Queryable")
ORCHID_DECLARE_INTERFACE(Orchid::Resource::IConfigurable,
	"org.orchid-wt.Orchid.ResourceInterfaces/Configurable")
Q_DECLARE_INTERFACE(Orchid::Resource::IConfigurable,
	"org.orchid-wt.Orchid.ResourceInterfaces/Configurable")
ORCHID_DECLARE_INTERFACE(Orchid::Resource::IAdvancedConfigurable,
	"org.orchid-wt.Orchid.ResourceInterfaces/AdvancedConfigurable");
Q_DECLARE_INTERFACE(Orchid::Resource::IAdvancedConfigurable,
	"org.orchid-wt.Orchid.ResourceInterfaces/AdvancedConfigurable")

#endif
