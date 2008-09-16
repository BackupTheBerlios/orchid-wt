#ifndef _ORCHID_FRAGMENTDOM_H_
#define _ORCHID_FRAGMENTDOM_H_

#include <QtCore/QtGlobal>
#include <QtCore/QVector>
#include <QtCore/QString>

#include <flower/documentenums.h>

namespace Orchid {

enum DomNodeType {
	DomUnknownType      = 0x0000,
	DomPCDATAType       = 0x0001,
	DomFragmentType     = 0x0002,
	DomTextType         = 0x0004,
	DomStructuralType   = 0x0008,
	DomHeadingType      = 0x0010,
	DomListType         = 0x0020,
	DomFlowContent      = DomPCDATAType | DomTextType | DomStructuralType | DomHeadingType,
	DomBodyContent      = DomHeadingType | DomStructuralType | DomListType,
	DomParagraphContent = DomPCDATAType | DomTextType | DomListType /* | insert others */,
	DomTextContent      = DomPCDATAType | DomTextType,
};
Q_DECLARE_FLAGS(DomNodeTypes, DomNodeType);

class DomNode {
public:
	virtual DomNodeType type() const;
};

class DomElement : public DomNode {
public:
	virtual Document::Tag tag() const;
	virtual bool append(DomNode* node);
	QVector<DomNode*> childs() const;
protected:
	void appendNode(DomNode* node);
private:
	QVector<DomNode*> m_childs;
};

class DomCharacters : public DomNode {
public:
	DomNodeType type() const;
	QString text() const;
	void setText(const QString& text);
private:
	QString m_text;
};

class DomSection : public DomElement {
public:
	Document::Tag tag() const;
	DomNodeType type() const;
	bool append(DomNode* node);
};

class DomHeading : public DomElement {
public:
	Document::Tag tag() const;
	virtual DomNodeType type() const;
	bool append(DomNode* node);
};

class DomParagraph : public DomElement {
public:
	Document::Tag tag() const;
	virtual DomNodeType type() const;
	bool append(DomNode* node);
};

class DomFragment : public DomElement {
public:
	DomNodeType type() const;
	bool append(DomNode* node);
};

class DomTextElement : public DomElement {
public:
	DomTextElement(Document::Tag tag);
public:
	Document::Tag tag() const;
	DomNodeType type() const;
	bool append(DomNode* node);
private:
	Document::Tag m_tag;
};

}

#endif
