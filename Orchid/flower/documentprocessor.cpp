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


#include "documentprocessor.h" 
#include "documentprocessor_p.h"

#include "style.h"
#include "styleattributes.h"

namespace Orchid {

/**
 * \class DocumentProcessor
 *
 * \brief DocumentProcessor provides an abstract interface processing
 * of documents.
 *
 * The DocumentProcessor defines a standard interface for processing
 * of documents. Those processings likely will be storing it in one
 * of different formats.
 *
 * A document source sends its processor to the processed by ...
 * TODO finish documentation
 *
 * \sa XHtml11StreamWriter, Tag
 */

/**
 * \fn void DocumentProcessor::startDocument(const DocumentHead &head) = 0
 *
 * Starts a new document with the document head \a head.
 */

/**
 * \fn void DocumentProcessor::endDocument() = 0
 *
 * Closes all opened elements and finishes the document.
 */

/**
 * \fn void DocumentProcessor::startElement(Document::Tag tag) = 0
 *
 * Starts a new element with the tag \a tag.
 */

/**
 * \fn void DocumentProcessor::endElement() = 0
 *
 * Closes the previous start element.
 */

/**
 * \fn void DocumentProcessor::insertCharacters(const QString& str) = 0
 *
 * Adds the text \a str at the current position of the document.
 * The implementations have to do the required escaping.
 */

/**
 * \fn void DocumentProcessor::setAttribute(Document::Attribute attr, const QVariant &val) = 0
 *
 * Sets the attribute \a attr for the next started element to \a val.
 */


DocumentProcessorPrivate::DocumentProcessorPrivate(DocumentProcessor* processor)
	: q_ptr(processor)
{
}

/**
 * Constructs a processor.
 */
DocumentProcessor::DocumentProcessor()
	: d_ptr(new DocumentProcessorPrivate(this))
{
}

/**
 * \internal
 */
DocumentProcessor::DocumentProcessor(DocumentProcessorPrivate* dd)
	: d_ptr(dd)
{
}

/**
 * Destructs the processor.
 */
DocumentProcessor::~DocumentProcessor() {
	delete d_ptr;
}

/**
 * Returns the binding for \a style in the context of the processor.
 */
StyleAttributes DocumentProcessor::attributes(const Style* style) {
	Q_D(DocumentProcessor);
	return d->styles.value(style);
}

/**
 * Adds \a style to the processor to be used with the prefix \a prefix.
 */
void DocumentProcessor::regStyle(const Style* style, const QString& prefix) {
	Q_D(DocumentProcessor);
	d->styles.insert(style, StyleAttributes(prefix));
}

/**
 * Returns the default string representation for \a role.
 */
QString DocumentProcessor::defaultRoleName(Document::Role role) {
	switch(role) {
		case Document::RoleDefinition:
			return "definition";
	}
	Q_ASSERT(false);
}

}
