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


#ifndef _ORCHID_IMAGERESOURCE_H_
#define _ORCHID_IMAGERESOURCE_H_

#include <QtCore/QtGlobal>
#include <stem/resource.h>

class QString;
class QImage;

namespace Orchid {

class ImageResourcePrivate;
class ImageResource :
	public Resource::Base,
	public Resource::IQueryable,
	public Resource::IConfigurable
{
	ORCHID_RESOURCE("Image")
public:
	ImageResource();
	ImageResource(const QString& path);
	ImageResource(const QImage& image);
	~ImageResource();
public:
	QString path() const;
	void setPath(const QString& path);
	QImage image() const;
	void setImage(const QImage& image);
	void query(Orchid::Request*);
	void setScaling(int sizeX, int sizeY);
	QList<Option> optionList() const;
	QVariant option(const QString&) const;
	bool setOption(const QString&, const QVariant&);
	virtual ImageResource *clone() const;
private:
	Q_DECLARE_PRIVATE(ImageResource)
};


}

#endif
