#include "htmlfragmentwriter.h"

#include "fragmentdom.h"

namespace Orchid {

class HtmlFragmentWriterPrivate {
public:
	HtmlFragmentWriterPrivate(HtmlFragmentWriter* writer);
public:
	void writeElement(DomElement* element);
protected:
	HtmlFragmentWriter* q_ptr;
private:
	DocumentProcessor* writer;
	Q_DECLARE_PUBLIC(HtmlFragmentWriter)
};

HtmlFragmentWriterPrivate::HtmlFragmentWriterPrivate(HtmlFragmentWriter* writer) {
	q_ptr = writer;
}

void HtmlFragmentWriterPrivate::writeElement(DomElement* element) {
	switch(element->tag()) {
		default:
			writer->writeStartElement(element->tag());
			break;
	}
	foreach(DomNode* child, element->childs()) {
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				writer->writeCharacters(chars->text());
			} break;
			default: {
				DomElement* element = dynamic_cast<DomElement*>(child);
				if(element) writeElement(element);
			} break;
		}
	}
	writer->writeEndElement();
}

HtmlFragmentWriter::HtmlFragmentWriter(DocumentProcessor* writer) {
	d_ptr = new HtmlFragmentWriterPrivate(this);
	Q_D(HtmlFragmentWriter);
	d->writer = writer;
}

HtmlFragmentWriter::~HtmlFragmentWriter() {
	delete d_ptr;
}

void HtmlFragmentWriter::write(DomFragment* fragment) {
	Q_D(HtmlFragmentWriter);
	
	foreach(DomNode* child, fragment->childs()) {
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				d->writer->writeCharacters(chars->text());
			} break;
			default: {
				DomElement* element = dynamic_cast<DomElement*>(child);
				if(element) d->writeElement(element);
			} break;
		}
	}
	return;
}

}
