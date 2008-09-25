#include "docstreamsplugin.h"

#include "docstreamsplugin.moc"

#include <stem/resourcefactory.h>
#include "docstreamssample.h"

using namespace Orchid;

DocStreamsPlugin::DocStreamsPlugin() {
	m_helpers << new ResourceFactoryHelper<DocStreamsSample>();
}

DocStreamsPlugin::~DocStreamsPlugin() {
	foreach(FactoryHelper *helper, m_helpers)
		delete helper;
}

QList<FactoryHelper*> DocStreamsPlugin::helpers() const {
	return m_helpers;
}

Q_EXPORT_PLUGIN2(document_streams_sample_extension, DocStreamsPlugin)
