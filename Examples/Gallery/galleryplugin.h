#ifndef _ORCHID_GALLERYPLUGIN_H_
#define _ORCHID_GALLERYPLUGIN_H_

#include <stem/extensionplugin.h>

class GalleryResourceFactory;
class GalleryPlugin : public Orchid::ExtensionPlugin {
	Q_OBJECT
public:
	GalleryPlugin();
	~GalleryPlugin();
public:
	QList<Orchid::FactoryHelper*> helpers() const;
private:
	GalleryResourceFactory *factory;
};

#endif
