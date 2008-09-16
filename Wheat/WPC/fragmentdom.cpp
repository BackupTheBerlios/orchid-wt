#include "fragmentdom.h"

#include <QtDebug>

namespace Orchid {

DomNodeType DomNode::type() const {
	return DomUnknownType;
}

Document::Tag DomElement::tag() const {
	return Document::TagUnknown;
}

bool DomElement::append(DomNode* child) {
	return false;
}

QVector<DomNode*> DomElement::childs() const {
	return m_childs;
}

void DomElement::appendNode(DomNode* node) {
	m_childs.append(node);
}

DomNodeType DomCharacters::type() const {
	return DomPCDATAType;
}

QString DomCharacters::text() const {
	return m_text;
}

void DomCharacters::setText(const QString& text) {
	m_text = text;
}

Document::Tag DomSection::tag() const {
	return Document::TagSection;
}

DomNodeType DomSection::type() const {
	return DomStructuralType;
}

bool DomSection::append(DomNode* node) {
	if(!DomNodeTypes(DomFlowContent).testFlag(node->type()))
		return false;
	
	appendNode(node);
	return true;
}

Document::Tag DomHeading::tag() const {
	return Document::TagHeading;
}

DomNodeType DomHeading::type() const {
	return DomHeadingType;
}

bool DomHeading::append(DomNode* node) {
	if(!DomNodeTypes(DomFlowContent).testFlag(node->type()))
	return false;
	
	appendNode(node);
	return true;
}

Document::Tag DomParagraph::tag() const {
	return Document::TagParagraph;
}

DomNodeType DomParagraph::type() const {
	return DomStructuralType;
}

bool DomParagraph::append(DomNode* node) {
	if(!DomNodeTypes(DomParagraphContent).testFlag(node->type()))
		return false;
	
	appendNode(node);
	return true;
}

DomNodeType DomFragment::type() const {
	return DomFragmentType;
}

bool DomFragment::append(DomNode* node) {
	if(!DomNodeTypes(DomBodyContent).testFlag(node->type()))
		return false;
	
	appendNode(node);
	return true;
}

DomTextElement::DomTextElement(Document::Tag tag) {
	m_tag = tag;
}

Document::Tag DomTextElement::tag() const {
	return m_tag;
}

DomNodeType DomTextElement::type() const {
	return DomTextType;
}

bool DomTextElement::append(DomNode* node) {
	if(!DomNodeTypes(DomTextContent).testFlag(node->type()))
		return false;

	// TODO add check for nested Document::TagTextLine-elements
	
	appendNode(node);
	return true;
}

}
