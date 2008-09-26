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


#ifndef _ORCHID_EXTENSIONPLUGIN_H_
#define _ORCHID_EXTENSIONPLUGIN_H_

#include <QtCore/QList>
#include <QtCore/QtPlugin>

namespace Orchid {

class FactoryHelper;

class ExtensionPluginInterface {
public:
	virtual ~ExtensionPluginInterface() {}
	virtual QList<FactoryHelper*> helpers() const = 0;
};

}

Q_DECLARE_INTERFACE(Orchid::ExtensionPluginInterface, "org.orchid-wt.Orchid.ExtensionPlugin/1.0")

namespace Orchid {

// An abstract base-class for plugins

class ExtensionPlugin : public QObject, public ExtensionPluginInterface {
public:
	Q_OBJECT
	Q_INTERFACES(Orchid::ExtensionPluginInterface)
};

}

#endif
