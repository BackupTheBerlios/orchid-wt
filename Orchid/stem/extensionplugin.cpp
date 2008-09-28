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


#include "extensionplugin.h"

#include "extensionplugin.moc"

namespace Orchid {

/**
 * \class ExtensionPluginInterface
 *
 * \brief ExtensionPluginInterface is an interface that all
 * extension plugins must implement.
 */

/**
 * \fn QList<FactoryHelper*> ExtensionPluginInterface::helpers() const = 0
 *
 * The implementation shoud return the list of helpers it provides.
 */

/**
 * \class ExtensionPlugin
 *
 * \brief ExtensionPlugin provides an abstract base class you should
 * use for your extension plugins.
 */

}
