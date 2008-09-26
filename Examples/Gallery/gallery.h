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


#ifndef _ORCHID_GALLERY_H_
#define _ORCHID_GALLERY_H_

#include <stem/resource.h>

class GalleryPrivate;
class Gallery :
	public Orchid::Resource::Base,
	public Orchid::Resource::IQueryable,
	public Orchid::Resource::IDirectory,
	public Orchid::Resource::IConfigurable
{
	ORCHID_RESOURCE("Gallery")
public:
	Gallery();
	~Gallery();
public:
	void setTitle(const QString &title);
	void insertFile(const QString &name, const QString &file);
	
	void query(Orchid::Request*);
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString &name);
	QList<Option> optionList() const;
	QVariant option(const QString& name) const;
	bool setOption(const QString& name, const QVariant& value);
protected:
	GalleryPrivate *d_ptr;
private:
	Q_DECLARE_PRIVATE(Gallery)
};


#endif
