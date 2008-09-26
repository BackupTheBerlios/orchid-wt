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


#include "extensionmanager.h"

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QtDebug>

#include "resourcefactory.h"
#include "extensionplugin.h"

namespace Orchid {

/**
 * \class FactoryHelper
 *
 * \brief FactoryHelper provides an abstract base for all factory
 * helpers.
 *
 * Factory helpers are classes that implement the creation of
 * of Orchid objects. This is currently limited to resources.
 */

/**
 * \fn QString FactoryHelper::key() const = 0
 *
 * Returns the key of the object type the factory provides.
 */

/**
 * \class ExtensionManager
 *
 * \brief ExtensionManager provides an interface for loading and
 * using extensions to the set of Orchid classes.
 */

class ExtensionManagerPrivate {
public:
	ExtensionManagerPrivate();
public:
	QMap<QString,FactoryHelper*> lookup;
};

ExtensionManagerPrivate::ExtensionManagerPrivate() {
}

Q_GLOBAL_STATIC(ExtensionManagerPrivate, extensionManager);

/**
 * Loads the extension plugin with the filename \a path and
 * returns true or returns false in the case of an error.
 */
bool ExtensionManager::loadExtension(const QString &path) {
	ExtensionManagerPrivate *d = extensionManager();
	
	QPluginLoader loader(path);
	if(loader.load()) {
		QObject *plugin = loader.instance();
		ExtensionPluginInterface *iface = qobject_cast<ExtensionPluginInterface*>(plugin);
	
		if(iface) {
			QList<FactoryHelper*> helpers = iface->helpers();
			QList<FactoryHelper*>::const_iterator i;
			for(i = helpers.begin(); i != helpers.end(); ++i) {
				// TODO read category from helper
				d->lookup.insert((*i)->key(), *i);
			}
			return true;
		} else {
			qWarning() << "Plugin is not an Orchid-extension" << path;
		}
	} else {
		qWarning() << "Could not load plugin:" << loader.errorString();
	}
	return false;
}

/**
 * Returns a list of valid keys for \a category.
 *
 * \note \a category will currently be ignored as only resources
 * are available
 */
QStringList ExtensionManager::keys(const QString &category) {
	// TODO use category for lookups
	return extensionManager()->lookup.keys();
}

/**
 * Returns the helper for \a key in \a category.
 *
 * \note \a category will currently be ignored as only resources
 * are available
 */
FactoryHelper *ExtensionManager::helper(const QString &category, const QString &key) {
	// TODO use category for lookups
	return extensionManager()->lookup.value(key);
}

}
