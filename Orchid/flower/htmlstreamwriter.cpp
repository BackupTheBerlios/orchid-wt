#include "htmlstreamwriter.h" 

#include <QtXml/QXmlStreamWriter>
#include <QHash>
#include <QStack>
#include <QtCore/QVariant>

#include <QtDebug> // TODO rm

#include "style.h"
#include "styleattributes.h"

namespace Orchid {
	
class HtmlStreamWriterPrivate {
	Q_DECLARE_PUBLIC(HtmlStreamWriter)
public:
    HtmlStreamWriterPrivate(HtmlStreamWriter *writer) : q_ptr(writer) { }
public:
	QHash<const Style*, StyleAttributes> styles;
protected:
	HtmlStreamWriter* q_ptr;
};

HtmlStreamWriter::HtmlStreamWriter()
	: d_ptr(new HtmlStreamWriterPrivate(this))
{
}

HtmlStreamWriter::HtmlStreamWriter(HtmlStreamWriterPrivate* dd)
	: d_ptr(dd)
{
}

StyleAttributes HtmlStreamWriter::attributes(const Style* style) {
	Q_D(HtmlStreamWriter);
	return d->styles.value(style);
}

void HtmlStreamWriter::regStyle(const Style* style, const QString& prefix) {
	Q_D(HtmlStreamWriter);
	d->styles.insert(style, StyleAttributes(prefix));
}

QString HtmlStreamWriter::defaultRoleName(HtmlRole role) {
	switch(role) {
		case HtmlRoleDefinition:
			return "definition";
	}
	Q_ASSERT(false);
}

//            l s    ls     t tl  ts   tls
// Span  -> - a span a+span - a   span a+span
// Block -> - a div  a+div  p a+p p    a+p
// 


class XHtml11StreamWriterPrivate : public HtmlStreamWriterPrivate {
	Q_DECLARE_PUBLIC(XHtml11StreamWriter)
public:
	struct Entry {
		HtmlTag tag;
		bool autoFormat : 1;
		bool hasElement : 1;
		bool hasAnchor : 1;
	};
public:
	XHtml11StreamWriterPrivate(XHtml11StreamWriter* writer)
		: HtmlStreamWriterPrivate(writer)
	{
		section = 0;
	}
public:
	QIODevice* device;
	QXmlStreamWriter xml;
	int section;
	QStack<Entry> specialStack;
	QHash<HtmlAttribute, QVariant> attributes;
};

XHtml11StreamWriter::XHtml11StreamWriter(QIODevice* device)
	: HtmlStreamWriter(new XHtml11StreamWriterPrivate(this))
{
	Q_D(XHtml11StreamWriter);
	d->device = device;
	d->xml.setDevice(device);
}

void XHtml11StreamWriter::setDevice(QIODevice* device) {
	Q_D(XHtml11StreamWriter);
	d->device = device;
	d->xml.setDevice(device);
}

QXmlStreamWriter* XHtml11StreamWriter::xmlWriter() {
	Q_D(XHtml11StreamWriter);
	return &d->xml;
}

void XHtml11StreamWriter::nextLinksTo(const QString& url) {
}

void XHtml11StreamWriter::writeStartDocument(const HtmlHead& head) {
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

void XHtml11StreamWriter::writeEndDocument() {
	Q_D(XHtml11StreamWriter);
	d->xml.writeEndDocument();
}

void XHtml11StreamWriter::writeStartElement(HtmlTag tag) {
	Q_D(XHtml11StreamWriter);
	XHtml11StreamWriterPrivate::Entry entry;
	if(d->attributes.contains(HtmlAttributeRole)) {
		d->xml.writeComment("role=\""+defaultRoleName(static_cast<HtmlRole>(d->attributes.value(HtmlAttributeRole).toInt()))+"\"");
	}
	switch(tag) {
		case HtmlTagUnknown: break;
		case HtmlTagBlock: break;
		case HtmlTagSection:
			d->section++;
			break;
		case HtmlTagHeading:
			switch(d->section) {
				case 0: d->xml.writeStartElement("h1"); break;
				case 1: d->xml.writeStartElement("h2"); break;
				case 2: d->xml.writeStartElement("h3"); break;
				case 3: d->xml.writeStartElement("h4"); break;
				case 4: d->xml.writeStartElement("h5"); break;
				default: d->xml.writeStartElement("h6"); break;
			}
			break;
		case HtmlTagParagraph:
			entry.autoFormat = d->xml.autoFormatting();
			d->xml.writeStartElement("p");
			d->xml.setAutoFormatting(false);
			break;
		case HtmlTagTextAbbreviation:
			d->xml.writeStartElement("abbr");
			if(d->attributes.contains(HtmlAttributeInlineFullText)) {
				d->xml.writeAttribute("title", d->attributes.value(HtmlAttributeInlineFullText).toString());
			}
			break;
		case HtmlTagTextCode:
			d->xml.writeStartElement("code"); break;
		case HtmlTagTextDefinition:
			d->xml.writeStartElement("dfn"); break;
		case HtmlTagTextEmphasis:
			d->xml.writeStartElement("em"); break;
		case HtmlTagTextKeyboard:
			d->xml.writeStartElement("kbd"); break;
		case HtmlTagTextQuote:
			d->xml.writeStartElement("q"); break;
		case HtmlTagTextSample:
			d->xml.writeStartElement("samp"); break;
		case HtmlTagTextSpan:
			d->xml.writeStartElement("span"); break;
		case HtmlTagTextStrong:
			d->xml.writeStartElement("strong"); break;
		case HtmlTagTextSubscript:
			d->xml.writeStartElement("sub"); break;
		case HtmlTagTextSuperscript:
			d->xml.writeStartElement("sup"); break;
		case HtmlTagTextVariable:
			d->xml.writeStartElement("var"); break;
		default: d->xml.writeStartElement("span"); break;
	}
	if(d->attributes.contains(HtmlAttributeId)) {
		d->xml.writeAttribute("id", d->attributes.value(HtmlAttributeId).toString());
	}
	if(d->attributes.contains(HtmlAttributeClassname)) {
		d->xml.writeAttribute("class", d->attributes.value(HtmlAttributeClassname).toString());
	}
	if(d->attributes.contains(HtmlAttributeLanguage)) {
		d->xml.writeAttribute("xml", "lang", d->attributes.value(HtmlAttributeLanguage).toString());
	}
	entry.tag = tag;
	d->specialStack.push(entry);
	d->attributes.clear();
}

void XHtml11StreamWriter::writeEndElement() {
	Q_D(XHtml11StreamWriter);
	XHtml11StreamWriterPrivate::Entry entry(d->specialStack.pop());
	switch(entry.tag) {
		case HtmlTagUnknown: break;
		case HtmlTagBlock: break;
		case HtmlTagSection:
			d->section--;
			break;
		case HtmlTagParagraph:
			d->xml.writeEndElement();
			d->xml.setAutoFormatting(entry.autoFormat);
			break;
		default:
			d->xml.writeEndElement();
			break;
	}
}

void XHtml11StreamWriter::writeCharacters(const QString& str) {
	Q_D(XHtml11StreamWriter);
	d->xml.writeCharacters(str);
}

void XHtml11StreamWriter::setAttribute(HtmlAttribute attr, const QVariant& val) {
	Q_D(XHtml11StreamWriter);
	d->attributes.insert(attr, val);
}

}
