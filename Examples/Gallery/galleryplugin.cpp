#include "galleryplugin.h"

#include "galleryplugin.moc"

#include <stem/resourcefactory.h>
#include "gallerydemo.h"

using namespace Orchid;

GalleryPlugin::GalleryPlugin() {
	m_helpers << new ResourceFactoryHelper<Gallery>();
	m_helpers << new ResourceFactoryHelper<GalleryDemo>();
}

GalleryPlugin::~GalleryPlugin() {
	foreach(FactoryHelper *helper, m_helpers)
		delete helper;
}

QList<FactoryHelper*> GalleryPlugin::helpers() const {
	return m_helpers;
}

Q_EXPORT_PLUGIN2(examlpes_gallery_extension, GalleryPlugin)
