#ifndef _BAMBOO_HTMLSTREAMWRITER_H_
#define _BAMBOO_HTMLSTREAMWRITER_H_

#include <QtGlobal>
#include "htmlhead.h"

class QXmlStreamWriter;
class QIODevice;
class QString;
class QVariant;

namespace Orchid {
	
class StyleAttributes;
class Style;

enum HtmlTag {
	HtmlTagUnknown = 0,
	HtmlTagBlock = 1,
	HtmlTagSection,
	HtmlTagHeading,
	HtmlTagParagraph,
	HtmlTagTextAbbreviation,
	HtmlTagTextCode,
	HtmlTagTextDefinition,
	HtmlTagTextEmphasis,
	HtmlTagTextKeyboard,
	HtmlTagTextQuote,
	HtmlTagTextSample,
	HtmlTagTextSpan,
	HtmlTagTextStrong,
	HtmlTagTextSubscript,
	HtmlTagTextSuperscript,
	HtmlTagTextVariable,
	HtmlTagCount // Only used for dimensioning lookups
};

enum HtmlAttribute {
	HtmlAttributeInlineFullText = 1,
	HtmlAttributeRole,
	HtmlAttributeId,
	HtmlAttributeClassname,
	HtmlAttributeLanguage,
	AttributeListSize
};

enum HtmlRole {
	HtmlRoleDefinition = 1,
};

class HtmlStreamWriterPrivate;
class HtmlStreamWriter {
public:
	HtmlStreamWriter();
public:
	StyleAttributes attributes(const Style* style);
	void regStyle(const Style* style, const QString& prefix);
	static QString defaultRoleName(HtmlRole role);
public:
	virtual void nextLinksTo(const QString& url) = 0;
	virtual void writeStartDocument(const HtmlHead &head = HtmlHead()) = 0;
	virtual void writeEndDocument() = 0;
	virtual void writeStartElement(HtmlTag tag) = 0;
	virtual void writeEndElement() = 0;
	virtual void writeCharacters(const QString& str) = 0;
	virtual void setAttribute(HtmlAttribute attr, const QVariant& val) = 0;
protected:
	HtmlStreamWriter(HtmlStreamWriterPrivate* dd);
private:
	Q_DECLARE_PRIVATE(HtmlStreamWriter)
protected:
	HtmlStreamWriterPrivate* d_ptr;
};

class XHtml11StreamWriterPrivate;
class XHtml11StreamWriter : public HtmlStreamWriter {
public:
	XHtml11StreamWriter(QIODevice* device = 0);
public:
	QXmlStreamWriter* xmlWriter();
	void setDevice(QIODevice* device);
public:
	void nextLinksTo(const QString& url);
	void writeStartDocument(const HtmlHead &head = HtmlHead());
	void writeEndDocument();
	void writeStartElement(HtmlTag special);
	void writeEndElement();
	void writeCharacters(const QString& str);
	void setAttribute(HtmlAttribute attr, const QVariant& val);
private:
	Q_DECLARE_PRIVATE(XHtml11StreamWriter)
};

}

#endif
