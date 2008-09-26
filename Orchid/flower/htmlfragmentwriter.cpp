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


#include "htmlfragmentwriter.h"

#include "documentprocessor.h"

#include "fragmentdom.h"

namespace Orchid {

// TODO refactor this and add documentation

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
			writer->startElement(element->tag());
			break;
	}
	foreach(DomNode* child, element->childs()) {
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				writer->insertCharacters(chars->text());
			} break;
			default: {
				DomElement* element = dynamic_cast<DomElement*>(child);
				if(element) writeElement(element);
			} break;
		}
	}
	writer->endElement();
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
				d->writer->insertCharacters(chars->text());
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
