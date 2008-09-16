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
