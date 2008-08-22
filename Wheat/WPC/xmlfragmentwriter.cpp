#include "xmlfragmentwriter.h"

#include <QtXml/QXmlStreamWriter>
#include "fragmentdom.h"

#include <QtCore/QtDebug>

namespace Orchid {

class XmlFragmentWriterHelper {
private:
	XmlFragmentWriterHelper();
public:
	static XmlFragmentWriterHelper* inst();
public:
	QString tagName(HtmlTag tag);
private:
	QVector<QString> m_lookup;
};

XmlFragmentWriterHelper* XmlFragmentWriterHelper::inst() {
	static XmlFragmentWriterHelper helper;
	return &helper;
}

XmlFragmentWriterHelper::XmlFragmentWriterHelper() {
	m_lookup.resize(HtmlTagCount);
	m_lookup[HtmlTagSection] = "section";
	m_lookup[HtmlTagHeading] = "h";
	m_lookup[HtmlTagParagraph] = "p";
	m_lookup[HtmlTagTextAbbreviation] = "abbr";
	m_lookup[HtmlTagTextCode] = "code";
	m_lookup[HtmlTagTextDefinition] = "dfn";
	m_lookup[HtmlTagTextEmphasis] = "em";
	m_lookup[HtmlTagTextKeyboard] = "kbd";
	m_lookup[HtmlTagTextQuote] = "q";
	m_lookup[HtmlTagTextSample] = "samp";
	m_lookup[HtmlTagTextSpan] = "span";
	m_lookup[HtmlTagTextStrong] = "strong";
	m_lookup[HtmlTagTextSubscript] = "sub";
	m_lookup[HtmlTagTextSuperscript] = "sup";
	m_lookup[HtmlTagTextVariable] = "var";
}

QString XmlFragmentWriterHelper::tagName(HtmlTag tag) {
	return m_lookup[tag];
}


class XmlFragmentWriterPrivate {
public:
	XmlFragmentWriterPrivate(XmlFragmentWriter* writer)
		: q_ptr(writer)
	{ }
public:
	void writeElement(QXmlStreamWriter* xml, DomElement* element);
protected:
	XmlFragmentWriter* q_ptr;
private:
	Q_DECLARE_PUBLIC(XmlFragmentWriter);
};

void XmlFragmentWriterPrivate::writeElement(QXmlStreamWriter* xml, DomElement* element) {
	XmlFragmentWriterHelper* helper = XmlFragmentWriterHelper::inst();
	switch(element->tag()) {
		default:
			xml->writeStartElement(helper->tagName(element->tag()));
	}
	
	foreach(DomNode* child, element->childs()) {
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				xml->writeCharacters(chars->text());
			} break;
			default: {
				DomElement* childElement = dynamic_cast<DomElement*>(child);
				if(childElement) writeElement(xml, childElement);
			}
		}
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
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				xml->writeCharacters(chars->text());
			} break;
			default: {
				DomElement* childElement = dynamic_cast<DomElement*>(child);
				if(childElement) d->writeElement(xml, childElement);
			}
		}
	}

	xml->writeEndElement();
	
	return true;
}


}
