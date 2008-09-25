#include "imageplugin.h"

#include "imageplugin.moc"

#include <stem/resourcefactory.h>
#include <leaf/imageresource.h>
#include <leaf/imagecollection.h>

using namespace Orchid;

ImagePlugin::ImagePlugin() {
	m_helpers << new ResourceFactoryHelper<ImageResource>();
	m_helpers << new ResourceFactoryHelper<ImageCollection>();
	m_helpers << new ResourceFactoryHelper<ImageCollectionScaling>();
}

ImagePlugin::~ImagePlugin() {
	foreach(FactoryHelper *helper, m_helpers)
		delete helper;
}

QList<FactoryHelper*> ImagePlugin::helpers() const {
	return m_helpers;
}

Q_EXPORT_PLUGIN2(orchid_imageres_extension, ImagePlugin)
