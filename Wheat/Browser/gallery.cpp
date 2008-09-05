#include "gallery.h"

#include <stem/location.h>
#include <stem/request.h>
#include <flower/htmlstreamwriter.h>
#include <QtXml/QXmlStreamWriter>

 
Gallery::Gallery() {
	insertModification("thumbs", new Orchid::ImageCollectionScaling(160, 160));
}

void Gallery::query(Orchid::Request *request) {
	if(!(request->method() & Orchid::GetMethod)) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;
	
	Orchid::XHtml11StreamWriter writer;
	writer.setDevice(request);
	writer.xmlWriter()->setAutoFormatting(true);
	
	writer.writeStartDocument();
	writer.writeStartElement(Orchid::HtmlTagHeading);
	writer.writeCharacters("Gallery");
	writer.writeEndElement();
	foreach(QString name, images()) {
		writer.xmlWriter()->writeStartElement("a");
		writer.xmlWriter()->writeAttribute("href", request->url(request->location().relative(name)));
		writer.xmlWriter()->writeEmptyElement("img");
		writer.xmlWriter()->writeAttribute("src", request->url(request->location().relative("thumbs/"+name)));
		writer.xmlWriter()->writeEndElement();
	}
	writer.writeEndDocument();
}
