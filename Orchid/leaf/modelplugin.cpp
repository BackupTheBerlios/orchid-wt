#include "modelplugin.h"

#include "modelplugin.moc"

#include <stem/resourcefactory.h>
#include <leaf/xmlmodelresource.h>

using namespace Orchid;

ModelPlugin::ModelPlugin() {
	m_helpers << new ResourceFactoryHelper<ModelResource>();
	m_helpers << new ResourceFactoryHelper<XmlModelResource>();
}

ModelPlugin::~ModelPlugin() {
	foreach(FactoryHelper *helper, m_helpers)
		delete helper;
}

QList<FactoryHelper*> ModelPlugin::helpers() const {
	return m_helpers;
}

Q_EXPORT_PLUGIN2(orchid_modelres_extension, ModelPlugin)
