#include "document.h"

#include "document.moc"

#include "fragment_p.h"

#include <QXmlStreamWriter>
#include "htmlstreamwriter.h"

#include <QHash>

#include "style.h"

namespace Bamboo {

class DocumentPrivate : public FragmentPrivate {
public:
	Q_DECLARE_PUBLIC(Document);
public:
	Fragment* mainFragment;
	QHash<Style*,QString> styleUrls;
};

Document::Document() : Fragment(*new DocumentPrivate) {
	Q_D(Document);
	d->mainFragment = 0;
}

void Document::build(HtmlStreamWriter* writer) {
	Q_D(Document);

	QString htmlNs = "http://www.w3.org/1999/xhtml";

	QXmlStreamWriter* xml = writer->xmlWriter();
	xml->setAutoFormatting(true);
	xml->writeStartDocument();
	xml->writeDTD("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" "
			"\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	xml->writeDefaultNamespace(htmlNs);
	xml->writeStartElement(htmlNs, "html");
	xml->writeStartElement(htmlNs, "head");
	xml->writeTextElement(htmlNs, "title", "testTitle");
	
	QHash<Style*, QString>::const_iterator i;
	for(i = d->styleUrls.constBegin(); i != d->styleUrls.constEnd(); ++i) {
		if(i.value().isEmpty()) {
			xml->writeStartElement("style");
			xml->writeAttribute("type", "text/css");
			xml->writeCharacters(i.key()->content());
			xml->writeEndElement();
			
		} else {
			xml->writeEmptyElement("link");
			xml->writeAttribute("rel", "stylesheet");
			xml->writeAttribute("type", "text/css");
			xml->writeAttribute("href", i.value());
		}
		writer->regStyle(i.key(), "");
	}

	xml->writeEndElement();
	xml->writeStartElement(htmlNs, "body");

	if(d->mainFragment) d->mainFragment->build(writer);

	xml->writeEndElement();
	xml->writeEndElement();
	xml->writeEndDocument();
}

void Document::setMainFragment(Fragment* fragment) {
	Q_D(Document);
	d->mainFragment = fragment;
}

void Document::addGlobalStyle(Style* style) {
	Q_D(Document);
	d->styleUrls.insert(style, QString());
}

void Document::addGlobalStyle(Style* style, const QString& url) {
	Q_D(Document);
	d->styleUrls.insert(style, url);
}

}
