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


#ifndef _ORCHID_IMAGECOLLECTION_P_H_
#define _ORCHID_IMAGECOLLECTION_P_H_

#include <QtCore/QHash>
#include <QtCore/QSet>

#include <stem/resourcebase_p.h>

namespace Orchid {

class ImageCollection;
class ImageCollectionMod;
class ImageCollectionScaling;

class ImageCollectionPrivate : public Resource::BasePrivate {
	Q_DECLARE_PUBLIC(ImageCollection)
public:
	ImageCollectionPrivate(ImageCollection* collection);
public:
	void resetFiles();
private:
	QStringList imageList;
	QStringList fileList;
	QHash<QString,QString> files;
	QSet<QString> mods;
};

class ImageCollectionModPrivate : public Resource::BasePrivate {
	Q_DECLARE_PUBLIC(ImageCollectionMod)
public:
	ImageCollectionModPrivate(ImageCollectionMod* mod);
public:
	void resetKeep();
private:
	ImageCollection* collection;
};

class ImageCollectionScalingPrivate : public ImageCollectionModPrivate {
public:
	ImageCollectionScalingPrivate(ImageCollectionScaling* scaling);
private:
	Q_DECLARE_PUBLIC(ImageCollectionScaling)
	int width, height;
};

}

#endif
