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


#include "xhtmlstreamwriter.h"

#include "documentprocessor_p.h"

#include "style.h"
#include "styleattributes.h"

#include <QtCore/QStack>
#include <QtCore/QVariant>
#include <QtXml/QXmlStreamWriter>

namespace Orchid {

//            l s    ls     t tl  ts   tls
// Span  -> - a span a+span - a   span a+span
// Block -> - a div  a+div  p a+p p    a+p
// 

/**
 * \class XHtml11StreamWriter
 *
 * \brief XHtml11StreamWriter provides an implementation of DocumentProcessor
 * for generating XHtml 1.1.
 *
 * XHtml11StreamWriter is an implementation of DocumentProcessor which
 * writes an XHtml 1.1 representation of the document to a device.
 *
 * \sa DocumentProcessor
 */

class XHtml11StreamWriterPrivate : public DocumentProcessorPrivate {
	Q_DECLARE_PUBLIC(XHtml11StreamWriter)
public:
	struct Entry {
		Document::Tag tag;
		bool autoFormat : 1;
		bool hasElement : 1;
		bool hasAnchor : 1;
	};
public:
	XHtml11StreamWriterPrivate(XHtml11StreamWriter* writer)
		: DocumentProcessorPrivate(writer)
	{
		section = 0;
	}
public:
	QIODevice* device;
	QXmlStreamWriter xml;
	int section;
	QStack<Entry> tagStack;
	QHash<Document::Attribute, QVariant> attributes;
};

/**
 * Constructs a stream writer which outputs to \a device.
 */
XHtml11StreamWriter::XHtml11StreamWriter(QIODevice* device)
	: DocumentProcessor(new XHtml11StreamWriterPrivate(this))
{
	Q_D(XHtml11StreamWriter);
	d->device = device;
	d->xml.setDevice(device);
}

/**
 * Sets the output device to \a device.
 */
void XHtml11StreamWriter::setDevice(QIODevice* device) {
	Q_D(XHtml11StreamWriter);
	d->device = device;
	d->xml.setDevice(device);
}

/**
 * Returns the xml stream used by the writer.
 *
 * \deprecated Use the interface of DocumentProcessor instead.
 */
// TODO remove
QXmlStreamWriter* XHtml11StreamWriter::xmlWriter() {
	Q_D(XHtml11StreamWriter);
	return &d->xml;
}

void XHtml11StreamWriter::startDocument(const DocumentHead& head) {
	Q_D(XHtml11StreamWriter);
	
	QString htmlNs = "http://www.w3.org/1999/xhtml";

	d->xml.writeStartDocument();
	d->xml.writeDTD("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\""
			" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	d->xml.writeDefaultNamespace(htmlNs);
	d->xml.writeStartElement(htmlNs, "html");
	d->xml.writeStartElement(htmlNs, "head");
	d->xml.writeTextElement(htmlNs, "title", head.title());
	
	typedef QVector<QPair<QString,Style*> > StyleList;
	StyleList styleUrls = head.styleUrls();
	StyleList::const_iterator i;
	for(i = styleUrls.constBegin(); i != styleUrls.constEnd(); ++i) {
		if(i->first.isEmpty()) {
			d->xml.writeStartElement("style");
			d->xml.writeAttribute("type", "text/css");
			d->xml.writeCharacters(i->second->content());
			d->xml.writeEndElement();
			
		} else {
			d->xml.writeEmptyElement("link");
			d->xml.writeAttribute("rel", "stylesheet");
			d->xml.writeAttribute("type", "text/css");
			d->xml.writeAttribute("href", i->first);
		}
		regStyle(i->second, "");
	}
	
	d->xml.writeEndElement();
	d->xml.writeStartElement(htmlNs, "body");
}

void XHtml11StreamWriter::endDocument() {
	Q_D(XHtml11StreamWriter);
	d->xml.writeEndDocument();
}

void XHtml11StreamWriter::startElement(Document::Tag tag) {
	Q_D(XHtml11StreamWriter);
	XHtml11StreamWriterPrivate::Entry entry;
	if(d->attributes.contains(Document::AttributeRole)) {
		d->xml.writeComment("role=\""+defaultRoleName(static_cast<Document::Role>(d->attributes.value(Document::AttributeRole).toInt()))+"\"");
	}
	switch(tag) {
		case Document::TagUnknown: break;
		case Document::TagBlock: break;
		case Document::TagSection:
			d->section++;
			break;
		case Document::TagHeading:
			switch(d->section) {
				case 0: d->xml.writeStartElement("h1"); break;
				case 1: d->xml.writeStartElement("h2"); break;
				case 2: d->xml.writeStartElement("h3"); break;
				case 3: d->xml.writeStartElement("h4"); break;
				case 4: d->xml.writeStartElement("h5"); break;
				default: d->xml.writeStartElement("h6"); break;
			}
			break;
		case Document::TagParagraph:
			entry.autoFormat = d->xml.autoFormatting();
			d->xml.writeStartElement("p");
			d->xml.setAutoFormatting(false);
			break;
		case Document::TagTextAbbreviation:
			d->xml.writeStartElement("abbr");
			if(d->attributes.contains(Document::AttributeInlineFullText)) {
				d->xml.writeAttribute("title", d->attributes.value(Document::AttributeInlineFullText).toString());
			}
			break;
		case Document::TagTextCode:
			d->xml.writeStartElement("code"); break;
		case Document::TagTextDefinition:
			d->xml.writeStartElement("dfn"); break;
		case Document::TagTextEmphasis:
			d->xml.writeStartElement("em"); break;
		case Document::TagTextKeyboard:
			d->xml.writeStartElement("kbd"); break;
		case Document::TagTextQuote:
			d->xml.writeStartElement("q"); break;
		case Document::TagTextSample:
			d->xml.writeStartElement("samp"); break;
		case Document::TagTextSpan:
			d->xml.writeStartElement("span"); break;
		case Document::TagTextStrong:
			d->xml.writeStartElement("strong"); break;
		case Document::TagTextSubscript:
			d->xml.writeStartElement("sub"); break;
		case Document::TagTextSuperscript:
			d->xml.writeStartElement("sup"); break;
		case Document::TagTextVariable:
			d->xml.writeStartElement("var"); break;
		default: d->xml.writeStartElement("span"); break;
	}
	if(d->attributes.contains(Document::AttributeId)) {
		d->xml.writeAttribute("id", d->attributes.value(Document::AttributeId).toString());
	}
	if(d->attributes.contains(Document::AttributeClassname)) {
		d->xml.writeAttribute("class", d->attributes.value(Document::AttributeClassname).toString());
	}
	if(d->attributes.contains(Document::AttributeLanguage)) {
		d->xml.writeAttribute("xml", "lang", d->attributes.value(Document::AttributeLanguage).toString());
	}
	entry.tag = tag;
	d->tagStack.push(entry);
	d->attributes.clear();
}

void XHtml11StreamWriter::endElement() {
	Q_D(XHtml11StreamWriter);
	XHtml11StreamWriterPrivate::Entry entry(d->tagStack.pop());
	switch(entry.tag) {
		case Document::TagUnknown: break;
		case Document::TagBlock: break;
		case Document::TagSection:
			d->section--;
			break;
		case Document::TagParagraph:
			d->xml.writeEndElement();
			d->xml.setAutoFormatting(entry.autoFormat);
			break;
		default:
			d->xml.writeEndElement();
			break;
	}
}

void XHtml11StreamWriter::insertCharacters(const QString& str) {
	Q_D(XHtml11StreamWriter);
	d->xml.writeCharacters(str);
}

void XHtml11StreamWriter::setAttribute(Document::Attribute attr, const QVariant& val) {
	Q_D(XHtml11StreamWriter);
	d->attributes.insert(attr, val);
}

}
