#include "modelplugin.h"

#include "modelplugin.moc"

#include <stem/resourcefactory.h>
#include <leaf/xmlmodelresource.h>

using namespace Orchid;

class ModelResourceFactory : public ResourceFactoryHelper {
	QStringList keys() const {
		QStringList list;
		list << "Model";
		list << "XmlModel";
		return list;
	}
	Resource::IResource *create(const QString &key) {
		Resource::IResource *res = 0;
		if(key == "Model") {
			res = new ModelResource();
		} else if(key == "XmlModel") {
			res = new XmlModelResource();
		}
		return res;
	}
};

ModelPlugin::ModelPlugin() {
	factory = new ModelResourceFactory();
}

ModelPlugin::~ModelPlugin() {
	delete factory;
}

QList<FactoryHelper*> ModelPlugin::helpers() const {
	return QList<FactoryHelper*>() << factory;
}

Q_EXPORT_PLUGIN2(orchid_modelres_extension, ModelPlugin)
