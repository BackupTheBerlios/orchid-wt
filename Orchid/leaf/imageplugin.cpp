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


#include "imageplugin.h"

#include "imageplugin.moc"

#include <stem/resourcefactory.h>
#include <leaf/imageresource.h>
#include <leaf/imagecollection.h>

using namespace Orchid;

ImagePlugin::ImagePlugin() {
	m_helpers << new ResourceFactoryHelper<ImageResource>();
	m_helpers << new ResourceFactoryHelper<ImageCollection>();
	m_helpers << new ResourceFactoryHelper<ImageCollectionScaling>();
}

ImagePlugin::~ImagePlugin() {
	foreach(FactoryHelper *helper, m_helpers)
		delete helper;
}

QList<FactoryHelper*> ImagePlugin::helpers() const {
	return m_helpers;
}

Q_EXPORT_PLUGIN2(orchid_imageres_extension, ImagePlugin)
