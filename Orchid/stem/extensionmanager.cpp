#include "extensionmanager.h"

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QtDebug>

#include "resourcefactory.h"
#include "extensionplugin.h"

namespace Orchid {

class ExtensionManagerPrivate {
public:
	ExtensionManagerPrivate();
public:
	QMap<QString,FactoryHelper*> lookup;
};

ExtensionManagerPrivate::ExtensionManagerPrivate() {
}

Q_GLOBAL_STATIC(ExtensionManagerPrivate, extensionManager);

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

QStringList ExtensionManager::keys(const QString &category) {
	// TODO use category for lookups
	return extensionManager()->lookup.keys();
}

FactoryHelper *ExtensionManager::helper(const QString &category, const QString &key) {
	// TODO use category for lookups
	return extensionManager()->lookup.value(key);
}

}
