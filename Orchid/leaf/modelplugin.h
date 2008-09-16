#ifndef _ORCHID_IMAGEPLUGIN_H_
#define _ORCHID_IMAGEPLUGIN_H_

#include <stem/extensionplugin.h>

class ModelResourceFactory;
class ModelPlugin : public Orchid::ExtensionPlugin {
	Q_OBJECT
	public:
		ModelPlugin();
		~ModelPlugin();
	public:
		QList<Orchid::FactoryHelper*> helpers() const;
	private:
		ModelResourceFactory *factory;
};

#endif
 
