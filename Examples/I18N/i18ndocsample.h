/*
 * This file is part of the Orchid example plugins
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


#ifndef _ORCHID_I18NDOCSAMPLE_H_
#define _ORCHID_I18NDOCSAMPLE_H_

#include <stem/resource.h>
#include <flower/documenthead.h>

namespace Orchid {
class Fragment;
}

class I18nDocSample : public Orchid::Resource::Base, public Orchid::Resource::IQueryable {
	ORCHID_RESOURCE("I18N-Document-Sample")
public:
	I18nDocSample();
	~I18nDocSample();
public:
	void query(Orchid::Request* request);
private:
	Orchid::DocumentHead m_head;
	Orchid::Fragment* m_body;
};

#endif
