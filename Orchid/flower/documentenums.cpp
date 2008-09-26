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


// NOTE This File contains only documentation for the enums in Document
// namespace and will not be compiled!

#include "documentenums.h"

namespace Orchid {

namespace Document {

/**
 * \enum Orchid::Document::Tag
 *
 * \brief Tag specifies possible tags that can be used in documents.
 *
 * The structure of documents and the meanings of tags are borrowed
 * from the XHTML 2.0 Working Draft.
 *
 * \note The meaning of the tags might change in future versions.
 *
 * \sa Attribute, DocumentProcessor
 */

/**
 * \var Tag TagUnknown
 *
 * An unknown tag. This value should only used for reporting errors.
 */

/**
 * \var Tag TagBlock
 *
 * Currently not used. Will be used to block together multiple
 * block level elements. Blocks are block level.
 */

/**
 * \var Tag TagSection
 *
 * Identifies a section or subsection. Sections are block level.
 */

/**
 * \var Tag TagHeading
 *
 * Identifies a heading. The level of the heading is indicated by the depth
 * of nested sections. Headings are block level but their content
 * is inline.
 */

/**
 * \var Tag TagParagraph
 *
 * Identifies a paragraph. Paragraphs are block level but their content 
 * is inline.
 */

/**
 * \var Tag TagTextAbbreviation
 *
 * Identifies a span marking an abbreviation. Use the
 * AttributeInlineFullText attribute to add its long form. Abbreviations
 * are inline.
 */

/**
 * \var Tag TagTextCode
 *
 * Identifies a span of code. Code is inline.
 */

/**
 * \var Tag TagTextDefinition
 *
 * Identifies a span marking a definition. Defintions are inline.
 */

/**
 * \var Tag TagTextEmphasis
 *
 * Identifies an emphasised span. Emphasis is inline.
 */

/**
 * \var Tag TagTextKeyboard
 *
 * Identifies an span of keyboard input. Keyboard input is inline.
 */

/**
 * \var Tag TagQuote
 *
 * Not used yet.
 */

/**
 * \var Tag TagSample
 *
 * Identifies an span of sample output from pragramms, scripts, etc. Samples
 * are inline.
 */

/**
 * \var Tag TagSpan
 *
 * Identifies an span. Span have no special meaning and might be used to apply
 * attributes to multiple elements.
 */

/**
 * \var Tag TagStrong
 *
 * Identifies an span with higher importance. Strong elements are inline.
 */

/**
 * \var Tag TagSubscript
 *
 * Identifies an span which content should be regarded as subscript.
 * Subcripts are inline.
 */

/**
 * \var Tag TagSuperscript
 *
 * Identifies an span which content should be regarded as superscript.
 * Superscripts are inline.
 */

/**
 * \var Tag TagVariable
 *
 * Identifies a variable or programm argument. Variables are inline.
 */

/**
 * \var Tag TagCount
 *
 * Holds the count of different tags. This value itself is no tag.
 */


/**
 * \enum Attribute
 *
 * \brief Attribute specifies the possible attributes that can be used in
 * documents.
 *
 * All attributes are candidates to be replaced in later versions.
 *
 * \sa Tag, DocumentProcessor
 */

/**
 * \var Attribute AttributeRole
 *
 * Used to identify the role of an element. See the the
 * XHTML 2.0 Working Draft for more information on roles.
 */

/**
 * \var Attribute AttributeId
 *
 * An id attribute holds the id of an element.
 */

/**
 * \var Attribute AttributeClassname
 *
 * An classname attribute holds the name of an elements class.
 */

/**
 * \var Attribute AttributeLanguage
 *
 * An language attribute holds the language of the content of an element.
 * Values should conform to RFC3066 (http://www.rfc-editor.org/rfc/rfc3066.txt).
 */

/**
 * \var Attribute AttributeListSize
 *
 * Not used. Might be removed.
 */


/**
 * \enum Role
 * 
 * \brief Specifies the role of an element.
 */

/**
 * \var Role RoleDefinition
 *
 * Marks a element to contain a definition. If this element contains an element
 * with TagTextDefintion that element holds the term being defined.
 */
}

}
