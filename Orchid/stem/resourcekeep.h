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


#ifndef _RESOURCEKEEP_H_
#define _RESOURCEKEEP_H_

#include <QObject>

#include "resourceglobals.h"

namespace Orchid {
namespace Resource {

class Base;

enum KeepingFlag {
	KeepPersistant = 1,
	KeepLonglifed = 2,
};
Q_DECLARE_FLAGS(KeepingFlags, KeepingFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(KeepingFlags);

class Handle;
class KeepItem;
class KeepPrivate;
class Keep {
	friend class KeepPrivate;
	friend class Handle;
public:
	Keep();
	~Keep();
public:
	Handle acquireHandle(const QString& name);
	Handle tryAcquireHandle(const QString& name);
	bool reset(const QString& name);
	void resetAll();
private:
	KeepPrivate* d;
};

class Handle {
	friend class ::Orchid::Resource::Keep;
public:
	Handle();
	Handle(const Handle &other);
	~Handle();
public:
	bool isNull() const;
	bool isEmpty() const;
	Ownership ownership() const;
	Base* resource() const;
	bool init(Base* resource, Ownership ownership = OwnedPrivate, KeepingFlags flags = KeepingFlags());
	QString name() const;
	Handle& operator=(const Handle& other);
	bool operator==(const Handle& other) const;
	inline bool operator!=(const Handle& other) const;
private:
	explicit Handle(KeepItem* item);
private:
	KeepItem* m_item;
};

inline bool Handle::operator!=(const Handle& other) const {
	return !operator==(other);
}

}
}

uint qHash(const Orchid::Resource::Handle& handle);

#endif
