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


#ifndef _ORCHID_REQUEST_PRIVATE_H_
#define _ORCHID_REQUEST_PRIVATE_H_

#include <QString>

#include "location.h"
#include "resourcekeep.h"

namespace Orchid {

class ORCHID_STEM_EXPORT RequestPrivate {
	Q_DECLARE_PUBLIC(Request)
public:
    RequestPrivate(Request *request)
        : q_ptr(request)
    { }
protected:
    Request* q_ptr;
private:
	Resource::Handle resource;
	Resource::Location location;
	Resource::LocationLookup locations;
};

class ORCHID_STEM_EXPORT SimpleRequestPrivate : public RequestPrivate {
	Q_DECLARE_PUBLIC(SimpleRequest)
public:
	SimpleRequestPrivate(SimpleRequest *request)
		: RequestPrivate(request)
	{ }
public:
	QIODevice* readDevice;
	QIODevice* writeDevice;
};

}


#endif
