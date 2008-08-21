#include "fragmentdom.h"

#include <QtDebug>

namespace Orchid {

DomNodeType DomNode::type() const {
	return DomUnknownType;
}

HtmlTag DomElement::tag() const {
	return HtmlTagUnknown;
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

HtmlTag DomSection::tag() const {
	return HtmlTagSection;
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

HtmlTag DomHeading::tag() const {
	return HtmlTagHeading;
}

DomNodeType DomHeading::type() const {
	return DomHeadingType;
}

bool DomHeading::append(DomNode* node) {
	if(!DomNodeTypes(DomPCDATAType | DomFlowContent).testFlag(node->type()))
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

}
