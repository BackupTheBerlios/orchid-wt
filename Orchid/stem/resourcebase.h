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


#ifndef _ORCHID_RESOURCEBASE_H_
#define _ORCHID_RESOURCEBASE_H_

#include "globals.h"

#include <QtCore/QString>

// NOTE i hope this check works on other compilers too

#define ORCHID_RESOURCE(NAME) \
public: \
	template<class T> inline void checkForResourceMacro(T*) const { const T* p = this; } \
	inline static QString staticTypeName() { return NAME; } \
	virtual QString typeNameInternal() const { return NAME; } \
private:

namespace Orchid {

namespace Resource {
class Keep;
class KeepPrivate;

class Base;
inline void *orchid_interface_cast(Base *res, const char *name);

class BasePrivate;
class ORCHID_STEM_EXPORT Base {
	ORCHID_RESOURCE("Resource")
	Q_DECLARE_PRIVATE(Base)
	friend class KeepPrivate;
	friend void *orchid_interface_cast(Base *res, const char *name);
public:
	Base();
	virtual ~Base();
public:
	inline QString typeName() const;
protected:
	Keep *keep();
	Base(BasePrivate *d);
	virtual void *interfaceCast(const char *name);
protected:
	BasePrivate *d_ptr;
};

inline QString Base::typeName() const {
	return typeNameInternal();
}

}

}

#endif
