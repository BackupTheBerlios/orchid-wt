#ifndef _BAMBOO_HTMLSTREAMWRITER_H_
#define _BAMBOO_HTMLSTREAMWRITER_H_

#include <QtGlobal>

class QXmlStreamWriter;
class QIODevice;
class QString;
class QVariant;

namespace Bamboo {
	
class StyleAttributes;
class Style;

class HtmlStreamWriterPrivate;

enum HtmlSpecial {
	HtmlSpecialBlock = 1,
	HtmlSpecialSection,
	HtmlSpecialHeading,
	HtmlSpecialParagraph,
	HtmlSpecialTextCode,
	HtmlSpecialTextAbbreviation,
	HtmlSpecialTextDefinition,
	HtmlSpecialTextEmphasis,
	HtmlSpecialTextKeyboard,
	HtmlSpecialTextQuote,
	HtmlSpecialTextSample,
	HtmlSpecialTextSpan,
	HtmlSpecialTextStrong,
	HtmlSpecialTextSubscript,
	HtmlSpecialTextSuperscript,
	HtmlSpecialTextVariable
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

class HtmlStreamWriter {
public:
	HtmlStreamWriter();
	HtmlStreamWriter(QIODevice* device);
public:
	QXmlStreamWriter* xmlWriter();
	void setDevice(QIODevice* device);
public:
	StyleAttributes attributes(const Style* style);
	void regStyle(const Style* style, const QString& prefix);
	static QString defaultRoleName(HtmlRole role);
public:
	virtual void nextLinksTo(const QString& url) = 0;
	virtual void writeSimpleSpecial(HtmlSpecial special, const QString& text) = 0;
	virtual void writeBeginSpecial(HtmlSpecial special) = 0;
	virtual void writeEndSpecial() = 0;
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
	void nextLinksTo(const QString& url);
	void writeSimpleSpecial(HtmlSpecial special, const QString& text);
	void writeBeginSpecial(HtmlSpecial special);
	void writeEndSpecial();
	void writeCharacters(const QString& str);
	void setAttribute(HtmlAttribute attr, const QVariant& val);
private:
	Q_DECLARE_PRIVATE(XHtml11StreamWriter)
};

}

#endif
