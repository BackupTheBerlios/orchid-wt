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


#include "fragmentdom.h"

#include <QtDebug>

namespace Orchid {

/**
 * \enum DomNodeType
 *
 * \brief DomNodeType specifies possible node types in the DOM for orchid
 * documents.
 *
 * \sa Document, DocumentProcessor, DomFragment
 */

/**
 * \var DomNodeType DomUnknownType
 *
 * The type of the node is not known.
 * (Only used by DomNode)
 */

/**
 * \var DomNodeType DomPCDATAType
 *
 * The node represents characters.
 * (Used by DomCharacters)
 */

/**
 * \var DomNodeType DomFragmentType
 *
 * The node represents a full document fragment.
 * (Used by DomFragment)
 */

/**
 * \var DomNodeType DomTextType
 *
 * The node represents a element with textual data.
 * Common text elements are \<em\> or \<strong\>.
 * (Used by DomTextElement)
 */

/**
 * \var DomNodeType DomStructuralType
 *
 * The node represents a strucuring element like paragraphs and sections.
 * (Used by DomParagraph, DomSection)
 */

/**
 * \var DomNodeType DomHeadingType
 *
 * The node represents a heading.
 * (Used by DomHeading)
 */

/**
 * \var DomNodeType DomListType
 *
 * The node represents a list.
 * (Not used yet)
 */

/**
 * \var DomNodeType DomFlowContent
 *
 * Mask for checking for valid flow-content
 */

/**
 * \var DomNodeType DomBodyContent
 *
 * Mask for checking for valid content for the document body.
 */

/**
 * \var DomNodeType DomParagraphContent
 *
 * Mask for checking for valid content for a paragraph.
 */

/**
 * \var DomNodeType DomTextContent
 *
 * Mask for checking for valid content of text elements.
 */


/**
 * \class DomNode
 *
 * \brief DomNode is the base for all nodes in the DOM.
 */

/**
 * Returns the type of the node.
 */
DomNodeType DomNode::type() const {
	return DomUnknownType;
}


/**
 * \class DomElement
 *
 * \brief DomElement is the base for all elements in the DOM
 */

/**
 * Returns the tag of the element.
 */
Document::Tag DomElement::tag() const {
	return Document::TagUnknown;
}

/**
 * Appends a child node to the element and returns whether this
 * appending was succesfull
 */
bool DomElement::append(DomNode* child) {
	return false;
}

/**
 * Returns a list of all child nodes.
 */
QVector<DomNode*> DomElement::childs() const {
	return m_childs;
}

/**
 * Used to append a valid child node to the child list.
 */
void DomElement::appendNode(DomNode* node) {
	m_childs.append(node);
}

/**
 * \class DomCharacters
 *
 * \brief Represents character data in a DOM.
 */

/**
 * Returns DomPCDATAType.
 */
DomNodeType DomCharacters::type() const {
	return DomPCDATAType;
}

/**
 * Returns the content of the node.
 */
QString DomCharacters::text() const {
	return m_text;
}

/**
 * Sets the content of the node.
 */
void DomCharacters::setText(const QString& text) {
	m_text = text;
}

/**
 * \class DomSection
 *
 * Represents a section in the DOM.
 */

/**
 * Returns TagSection.
 */
Document::Tag DomSection::tag() const {
	return Document::TagSection;
}

/**
 * Returns DomStructuralType.
 */
DomNodeType DomSection::type() const {
	return DomStructuralType;
}

/**
 * If \a node is a valid section child (type in the DomFlowContent mask)
 * appends \a node to the child list when it is a valid section and returns true.
 * Otherwise returns false.
 */
bool DomSection::append(DomNode* node) {
	if(!DomNodeTypes(DomFlowContent).testFlag(node->type()))
		return false;
	
	appendNode(node);
	return true;
}

/**
 * \class DomHeading
 *
 * Represents a heading in the DOM.
 */

/**
 * Returns TagHeading.
 */
Document::Tag DomHeading::tag() const {
	return Document::TagHeading;
}

/**
 * Returns DomHeadingType.
 */
DomNodeType DomHeading::type() const {
	return DomHeadingType;
}

/**
 * If \a node is a valid heading child (type in the DomFlowContent mask)
 * appends \a node to the child list when it is a valid section and returns true.
 * Otherwise returns false.
 */
bool DomHeading::append(DomNode* node) {
	if(!DomNodeTypes(DomFlowContent).testFlag(node->type()))
	return false;
	
	appendNode(node);
	return true;
}

/**
 * \class DomParagraph
 *
 * Represents a paragraph in the DOM.
 */

/**
 * Returns TagParagraph.
 */
Document::Tag DomParagraph::tag() const {
	return Document::TagParagraph;
}

/**
 * Returns DomStructuralType.
 */
DomNodeType DomParagraph::type() const {
	return DomStructuralType;
}

/**
 * If \a node is a valid paragraph child (type in the DomParagraphContent mask)
 * appends \a node to the child list when it is a valid section and returns true.
 * Otherwise returns false.
 */
bool DomParagraph::append(DomNode* node) {
	if(!DomNodeTypes(DomParagraphContent).testFlag(node->type()))
		return false;
	
	appendNode(node);
	return true;
}

/**
 * \class DomFragment
 *
 * Represents a body fragment in the DOM.
 */

/**
 * Returns DomFragmentType.
 */
DomNodeType DomFragment::type() const {
	return DomFragmentType;
}

/**
 * If \a node is a valid body child (type in the DomBodyContent mask)
 * appends \a node to the child list when it is a valid section and returns true.
 * Otherwise returns false.
 */
bool DomFragment::append(DomNode* node) {
	if(!DomNodeTypes(DomBodyContent).testFlag(node->type()))
		return false;
	
	appendNode(node);
	return true;
}

/**
 * \class DomTextElement
 *
 * Represents a text element in the DOM.
 */

/**
 * Constructs a text element with the tag \a tag.
 */
DomTextElement::DomTextElement(Document::Tag tag) {
	m_tag = tag;
}

/**
 * Returns the tag of the text element.
 */
Document::Tag DomTextElement::tag() const {
	return m_tag;
}

/**
 * Returns DomTextType.
 */
DomNodeType DomTextElement::type() const {
	return DomTextType;
}

/**
 * If \a node is a valid text child (type in the DomTextContent mask)
 * appends \a node to the child list when it is a valid section and returns true.
 * Otherwise returns false.
 */
bool DomTextElement::append(DomNode* node) {
	if(!DomNodeTypes(DomTextContent).testFlag(node->type()))
		return false;

	// TODO add check for nested Document::TagTextLine-elements
	
	appendNode(node);
	return true;
}

}
