#include "document.h"

#include "document.moc"

#include "fragment_p.h"

#include <QXmlStreamWriter>
#include "builder.h"

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

void Document::build(Builder* builder) {
	Q_D(Document);

	QString htmlNs = "http://www.w3.org/1999/xhtml";

	QXmlStreamWriter* writer = builder->xml();
	writer->setAutoFormatting(true);
	writer->writeStartDocument();
	writer->writeDTD("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" "
			"\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
	writer->writeDefaultNamespace(htmlNs);
	writer->writeStartElement(htmlNs, "html");
	writer->writeStartElement(htmlNs, "head");
	writer->writeTextElement(htmlNs, "title", "testTitle");
	
	QHash<Style*, QString>::const_iterator i;
	for(i = d->styleUrls.constBegin(); i != d->styleUrls.constEnd(); ++i) {
		if(i.value().isEmpty()) {
			writer->writeStartElement("style");
			writer->writeAttribute("type", "text/css");
			writer->writeCharacters(i.key()->content());
			writer->writeEndElement();
			
		} else {
			writer->writeEmptyElement("link");
			writer->writeAttribute("rel", "stylesheet");
			writer->writeAttribute("type", "text/css");
			writer->writeAttribute("href", i.value());
		}
		builder->regStyle(i.key(), "");
	}

	writer->writeEndElement();
	writer->writeStartElement(htmlNs, "body");

	if(d->mainFragment) d->mainFragment->build(builder);

	writer->writeEndElement();
	writer->writeEndElement();
	writer->writeEndDocument();
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
