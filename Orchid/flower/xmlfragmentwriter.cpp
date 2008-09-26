/*
 * This file is part of the Orchid libraries
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "xmlfragmentwriter.h"

#include <QtXml/QXmlStreamWriter>
#include "fragmentdom.h"

#include <QtCore/QtDebug>

namespace Orchid {

/**
 * \class XmlFragmentWriter
 *
 * \brief Writes an xml representation of an DOM.
 *
 * \deprecated An newer writer based on DocumentProcessor is needed
 */

class XmlFragmentWriterHelper {
private:
	XmlFragmentWriterHelper();
public:
	static XmlFragmentWriterHelper* inst();
public:
	QString tagName(Document::Tag tag);
private:
	QVector<QString> m_lookup;
};

XmlFragmentWriterHelper* XmlFragmentWriterHelper::inst() {
	static XmlFragmentWriterHelper helper;
	return &helper;
}

XmlFragmentWriterHelper::XmlFragmentWriterHelper() {
	m_lookup.resize(Document::TagCount);
	m_lookup[Document::TagSection] = "section";
	m_lookup[Document::TagHeading] = "h";
	m_lookup[Document::TagParagraph] = "p";
	m_lookup[Document::TagTextAbbreviation] = "abbr";
	m_lookup[Document::TagTextCode] = "code";
	m_lookup[Document::TagTextDefinition] = "dfn";
	m_lookup[Document::TagTextEmphasis] = "em";
	m_lookup[Document::TagTextKeyboard] = "kbd";
	m_lookup[Document::TagTextQuote] = "q";
	m_lookup[Document::TagTextSample] = "samp";
	m_lookup[Document::TagTextSpan] = "span";
	m_lookup[Document::TagTextStrong] = "strong";
	m_lookup[Document::TagTextSubscript] = "sub";
	m_lookup[Document::TagTextSuperscript] = "sup";
	m_lookup[Document::TagTextVariable] = "var";
}

QString XmlFragmentWriterHelper::tagName(Document::Tag tag) {
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
