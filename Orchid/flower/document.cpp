#include "document.h"

#include "document.moc"

#include "fragment_p.h"

#include <QXmlStreamWriter>
#include "htmlstreamwriter.h"

#include <QHash>

#include "style.h"

namespace Orchid {

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

	QXmlStreamWriter* xml = writer->xmlWriter();
	xml->setAutoFormatting(true);

	writer->writeStartDocument();

	if(d->mainFragment) d->mainFragment->build(writer);

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
