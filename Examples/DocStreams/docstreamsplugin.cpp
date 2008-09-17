#include "docstreamsplugin.h"

#include "docstreamsplugin.moc"

#include <stem/resourcefactory.h>
#include "docstreamssample.h"

using namespace Orchid;

class DocStreamsResourceFactory : public ResourceFactoryHelper {
	QStringList keys() const {
		QStringList list;
		list << "Document-Streams-Sample";
		return list;
	}
	Resource::Base *create(const QString &key) {
		Resource::Base *res = 0;
		if(key == "Document-Streams-Sample") {
			res = new DocStreamsSample();
		}
		return res;
	}
};

DocStreamsPlugin::DocStreamsPlugin() {
	factory = new DocStreamsResourceFactory();
}

DocStreamsPlugin::~DocStreamsPlugin() {
	delete factory;
}

QList<FactoryHelper*> DocStreamsPlugin::helpers() const {
	return QList<FactoryHelper*>() << factory;
}

Q_EXPORT_PLUGIN2(document_streams_sample_extension, DocStreamsPlugin)
