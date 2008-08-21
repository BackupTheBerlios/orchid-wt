#include "fragmentdom.h"

#include <QtDebug>

namespace Orchid {

DomNodeType DomNode::type() const {
	return DomUnknownType;
}

HtmlSpecial DomElement::tag() const {
	return HtmlSpecialUnknown;
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

HtmlSpecial DomSection::tag() const {
	return HtmlSpecialSection;
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

HtmlSpecial DomHeading::tag() const {
	return HtmlSpecialHeading;
}

DomNodeType DomHeading::type() const {
	return DomHeadingType;
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
