#include "xmlmodelresource.h"

#include "modelresource_p.h"

#include <QtXml/QXmlStreamWriter>
#include <stem/request.h>
#include <stem/location.h>
#include <QtCore/QAbstractItemModel>

namespace Orchid {

class XmlModelResourcePrivate : public ModelResourcePrivate {
public:
	XmlModelResourcePrivate(XmlModelResource* res);
private:
	Q_DECLARE_PUBLIC(XmlModelResource)
};

XmlModelResourcePrivate::XmlModelResourcePrivate(XmlModelResource* res)
	: ModelResourcePrivate(res)
{
}

XmlModelResource::XmlModelResource(QAbstractItemModel* model)
	: ModelResource(new XmlModelResourcePrivate(this), model)
{
}

XmlModelResource::~XmlModelResource() {
}

void XmlModelResource::query(Orchid::Request* request) {
	query(request, QModelIndex());
}

void XmlModelResource::query(Orchid::Request* request, const QModelIndex& index) {
	if(!model()) return;

	if(!request->open(QIODevice::ReadWrite)) return;
	QXmlStreamWriter xml(request);

	QString qt("qt-orchid-mapping");
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeNamespace(qt, "qt");
	xml.writeStartElement(qt, "item-model");
	int rows = model()->rowCount(index);
	int cols = model()->columnCount(index);
	for(int i = 0; i < rows; ++i) {
		xml.writeStartElement(qt, "row");
		xml.writeAttribute("pos", QString::number(i));
		for(int j = 0; j < cols; ++j) {
			QModelIndex child = model()->index(i,j, index);
			QString url = request->url(request->location().relative(name(child)));
			xml.writeStartElement(qt, "cell");
			xml.writeAttribute("pos", QString::number(j));
			xml.writeAttribute("href", url);
			xml.writeStartElement(qt, "data");
			xml.writeAttribute("role", "display");
			xml.writeCDATA(child.data(Qt::DisplayRole).toString());
			xml.writeEndElement();
			xml.writeEndElement();
		}
		xml.writeEndElement();
	}
	xml.writeEndDocument();
}

}
