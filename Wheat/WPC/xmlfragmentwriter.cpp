#include "xmlfragmentwriter.h"

#include <QtXml/QXmlStreamWriter>
#include "fragmentdom.h"

namespace Orchid {

class XmlFragmentWriterPrivate {
public:
	XmlFragmentWriterPrivate(XmlFragmentWriter* writer)
		: q_ptr(writer)
	{ }
public:
	static QString tagName(HtmlTag tag);
	void writeElement(QXmlStreamWriter* xml, DomElement* element);
protected:
	XmlFragmentWriter* q_ptr;
private:
	Q_DECLARE_PUBLIC(XmlFragmentWriter);
};

QString XmlFragmentWriterPrivate::tagName(HtmlTag tag) {
	switch(tag) {
		case HtmlTagSection: return "section";
		case HtmlTagHeading: return "h";
	}
}

void XmlFragmentWriterPrivate::writeElement(QXmlStreamWriter* xml, DomElement* element) {
	switch(element->tag()) {
		default:
			xml->writeStartElement(tagName(element->tag()));
	}
	
	foreach(DomNode* child, element->childs()) {
		DomElement* childElement = dynamic_cast<DomElement*>(child);
		if(childElement) writeElement(xml, childElement);
	}
	xml->writeEndElement();
}

XmlFragmentWriter::XmlFragmentWriter() {
	d_ptr = new XmlFragmentWriterPrivate(this);
}

XmlFragmentWriter::~XmlFragmentWriter() {
	delete d_ptr;
}

bool XmlFragmentWriter::write(QXmlStreamWriter* xml, DomFragment* fragment) {
	Q_D(XmlFragmentWriter);
	if(!fragment) return false;
	
	xml->writeStartElement("fragment");

	foreach(DomNode* child, fragment->childs()) {
		DomElement* childElement = dynamic_cast<DomElement*>(child);
		if(childElement) d->writeElement(xml, childElement);
	}

	xml->writeEndElement();
	
	return true;
}


}
