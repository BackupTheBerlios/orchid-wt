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


#include "fragmentbuilder.h"

#include "documentprocessor_p.h"
#include "styleattributes.h"
#include <QtCore/QStack>

#include "fragmentdom.h"

namespace Orchid {

using namespace Document;

/**
 * \class FragmentBuilder
 *
 * \brief FragmentBuilder provides an implementation of DocumentProcessor
 * for building a DOM of an document.
 *
 * FragmentBuilder builds an DomFragment for an document that can be used for
 * further processing.
 *
 * \sa DocumentProcessor
 */

class FragmentBuilderPrivate : public DocumentProcessorPrivate {
public:
	FragmentBuilderPrivate(FragmentBuilder *qq);
private:
	QStack<DomElement*> stack;
	DomFragment *fragment;
	bool finished;
	Q_DECLARE_PUBLIC(FragmentBuilder)
};

FragmentBuilderPrivate::FragmentBuilderPrivate(FragmentBuilder *qq)
	: DocumentProcessorPrivate(qq)
{
	fragment = 0;
	finished = true;
}

FragmentBuilder::FragmentBuilder()
	: DocumentProcessor(new FragmentBuilderPrivate(this))
{ }

FragmentBuilder::~FragmentBuilder() {
}

void FragmentBuilder::startDocument(const DocumentHead &head) {
	Q_D(FragmentBuilder);
	Q_ASSERT(!d->fragment);
	d->finished = false;
	d->fragment = new DomFragment();
	d->stack << d->fragment;
}

void FragmentBuilder::endDocument() {
	Q_D(FragmentBuilder);
	d->finished = true;
	d->stack.clear();
}

void FragmentBuilder::startElement(Document::Tag tag) {
	Q_D(FragmentBuilder);
	switch(tag) {
		case Document::TagUnknown: {
			Q_ASSERT(false);
		} break;
		case Document::TagBlock: {
			d->stack.push(d->stack.top());
		} break;
		case Document::TagSection: {
			DomSection *section = new DomSection();
			Q_ASSERT(d->stack.top()->append(section));
			d->stack.push(section);
		} break;
		case Document::TagHeading: {
			DomHeading *heading = new DomHeading();
			Q_ASSERT(d->stack.top()->append(heading));
			d->stack.push(heading);
		} break;
		case Document::TagParagraph: {
			DomParagraph *paragraph = new DomParagraph();
			Q_ASSERT(d->stack.top()->append(paragraph));
			d->stack.push(paragraph);
		} break;
		case Document::TagTextAbbreviation ... Document::TagTextVariable: {
			DomTextElement *text = new DomTextElement(tag);
			Q_ASSERT(d->stack.top()->append(text));
			d->stack.push(text);
		} break;
	}
}

void FragmentBuilder::endElement() {
	Q_D(FragmentBuilder);
	d->stack.pop();
}

void FragmentBuilder::insertCharacters(const QString &str) {
	Q_D(FragmentBuilder);
	DomCharacters *chars = new DomCharacters;
	chars->setText(str);
	Q_ASSERT(d->stack.top()->append(chars));
}

void FragmentBuilder::setAttribute(Attribute attr, const QVariant &val) {
	// TODO not used yet
}

/**
 * Returns the fragment constructed by the builder or 0 if no fragment
 * was constructed yet or a construction is under process.
 */
DomFragment *FragmentBuilder::fragment() const {
	Q_D(const FragmentBuilder);
	if(!d->finished) return 0;
	return d->fragment;
}

/**
 * Returns the fragment constructed by the builder and sets it to 0. If no
 * fragment was constructed yet or a construction is under process this
 * function returns 0.
 */
DomFragment *FragmentBuilder::takeFragment() {
	Q_D(FragmentBuilder);
	if(!d->finished) return 0;
	DomFragment *fragment = d->fragment;
	d->fragment = 0;
	return fragment;
}

}
