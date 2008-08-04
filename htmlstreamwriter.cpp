#include "htmlstreamwriter.h" 

#include <QtXml/QXmlStreamWriter>
#include <QHash>
#include <QStack>

#include "styleattributes.h"

namespace Bamboo {

class HtmlStreamWriterPrivate {
	Q_DECLARE_PUBLIC(HtmlStreamWriter)
public:
    HtmlStreamWriterPrivate(HtmlStreamWriter *writer) : q_ptr(writer) {
		device = 0;
	}
public:
	QIODevice* device;
	QXmlStreamWriter xml;
	QHash<const Style*, StyleAttributes> styles;
protected:
	HtmlStreamWriter* q_ptr;
};

HtmlStreamWriter::HtmlStreamWriter()
	: d_ptr(new HtmlStreamWriterPrivate(this))
{
}

HtmlStreamWriter::HtmlStreamWriter(QIODevice* device)
	: d_ptr(new HtmlStreamWriterPrivate(this))
{
	Q_D(HtmlStreamWriter);
	
	d->device = device;
	d->xml.setDevice(device);
}

HtmlStreamWriter::HtmlStreamWriter(HtmlStreamWriterPrivate* dd)
	: d_ptr(dd)
{
}

QXmlStreamWriter* HtmlStreamWriter::xmlWriter() {
	Q_D(HtmlStreamWriter);
	return &d->xml;
}

void HtmlStreamWriter::setDevice(QIODevice* device) {
	Q_D(HtmlStreamWriter);
	d->device = device;
	d->xml.setDevice(device);
}

StyleAttributes HtmlStreamWriter::attributes(const Style* style) {
	Q_D(HtmlStreamWriter);
	return d->styles.value(style);
}

void HtmlStreamWriter::regStyle(const Style* style, const QString& prefix) {
	Q_D(HtmlStreamWriter);
	d->styles.insert(style, StyleAttributes(prefix));
}

//            l s    ls     t tl  ts   tls
// Span  -> - a span a+span - a   span a+span
// Block -> - a div  a+div  p a+p p    a+p
// 


class XHtml11StreamWriterPrivate : public HtmlStreamWriterPrivate {
	Q_DECLARE_PUBLIC(XHtml11StreamWriter)
public:
	struct Entry {
		HtmlSpecial special;
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
	int section;
	QStack<Entry> specialStack;
};

XHtml11StreamWriter::XHtml11StreamWriter(QIODevice* device)
	: HtmlStreamWriter(new XHtml11StreamWriterPrivate(this))
{
	Q_D(XHtml11StreamWriter);
	d->device = device;
	d->xml.setDevice(device);
}

void XHtml11StreamWriter::nextLinksTo(const QString& url) {
}

void XHtml11StreamWriter::writeSimpleSpecial(HtmlSpecial special, const QString& text) {
	Q_D(XHtml11StreamWriter);
	switch(special) {
		case HtmlSpecialBlock:
			d->xml.writeCharacters(text);
			break;
		case HtmlSpecialSection:
			d->xml.writeTextElement("p", text);
			break;
		default:
			writeBeginSpecial(special);
			d->xml.writeCharacters(text);
			writeEndSpecial();
			break;
	}
}

void XHtml11StreamWriter::writeBeginSpecial(HtmlSpecial special) {
	Q_D(XHtml11StreamWriter);
	switch(special) {
		case HtmlSpecialBlock: break;
		case HtmlSpecialSection:
			d->section++;
			break;
		case HtmlSpecialHeading:
			switch(d->section) {
				case 0: d->xml.writeStartElement("h1"); break;
				case 1: d->xml.writeStartElement("h2"); break;
				case 2: d->xml.writeStartElement("h3"); break;
				case 3: d->xml.writeStartElement("h4"); break;
				case 4: d->xml.writeStartElement("h5"); break;
				default: d->xml.writeStartElement("h6"); break;
			}
			break;
	}
	XHtml11StreamWriterPrivate::Entry entry;
	entry.special = special;
	d->specialStack.push(entry);
	
}

void XHtml11StreamWriter::writeEndSpecial() {
	Q_D(XHtml11StreamWriter);
	XHtml11StreamWriterPrivate::Entry entry(d->specialStack.pop());
	switch(entry.special) {
		case HtmlSpecialBlock: break;
		case HtmlSpecialSection:
			d->section--;
			break;
		default:
			d->xml.writeEndElement();
			break;
	}
}

}
