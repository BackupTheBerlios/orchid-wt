#include "document.h"

#include "document.moc"

#include "fragment_p.h"

#include <QXmlStreamWriter>
#include "builder.h"
#include "dependency.h"


namespace Bamboo {

class DocumentPrivate : public FragmentPrivate {
public:
	Q_DECLARE_PUBLIC(Document);
public:
	Fragment* mainFragment;
};

Document::Document() : Fragment(*new DocumentPrivate) {
	Q_D(Document);
	d->mainFragment = 0;
}

void Document::build(Builder* builder) {
	Q_D(Document);

	Dependencies deps;
	if(d->mainFragment) d->mainFragment->getDependencies(deps);

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
	
	foreach(Dependency dep, deps) {
		dep.writeToHead(builder);
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

}
