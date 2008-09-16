#ifndef _ORCHID_IMAGEPLUGIN_H_
#define _ORCHID_IMAGEPLUGIN_H_

#include <stem/extensionplugin.h>

class ImageResourceFactory;
class ImagePlugin : public Orchid::ExtensionPlugin {
	Q_OBJECT
public:
	ImagePlugin();
	~ImagePlugin();
public:
	QList<Orchid::FactoryHelper*> helpers() const;
private:
	ImageResourceFactory *factory;
};

#endif
