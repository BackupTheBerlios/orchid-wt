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


#ifndef _ORCHID_GALLERYPLUGIN_H_
#define _ORCHID_GALLERYPLUGIN_H_

#include <stem/extensionplugin.h>

class GalleryPlugin : public Orchid::ExtensionPlugin {
	Q_OBJECT
public:
	GalleryPlugin();
	~GalleryPlugin();
public:
	QList<Orchid::FactoryHelper*> helpers() const;
private:
	QList<Orchid::FactoryHelper*> m_helpers;
};

#endif
