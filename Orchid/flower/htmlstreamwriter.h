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

namespace Document {

enum Tag {
	TagUnknown = 0,
	TagBlock = 1,
	TagSection,
	TagHeading,
	TagParagraph,
	TagTextAbbreviation,
	TagTextCode,
	TagTextDefinition,
	TagTextEmphasis,
	TagTextKeyboard,
	TagTextQuote,
	TagTextSample,
	TagTextSpan,
	TagTextStrong,
	TagTextSubscript,
	TagTextSuperscript,
	TagTextVariable,
	TagCount // Only used for dimensioning lookups
};

enum Attribute {
	AttributeInlineFullText = 1,
	AttributeRole,
	AttributeId,
	AttributeClassname,
	AttributeLanguage,
	AttributeListSize
};

enum Role {
	RoleDefinition = 1,
};

}

class DocumentProcessorPrivate;
class DocumentProcessor {
public:
	DocumentProcessor();
public:
	StyleAttributes attributes(const Style* style);
	void regStyle(const Style* style, const QString& prefix);
	static QString defaultRoleName(Document::Role role);
public:
	virtual void nextLinksTo(const QString& url) = 0;
	virtual void writeStartDocument(const DocumentHead &head = DocumentHead()) = 0;
	virtual void writeEndDocument() = 0;
	virtual void writeStartElement(Document::Tag tag) = 0;
	virtual void writeEndElement() = 0;
	virtual void writeCharacters(const QString& str) = 0;
	virtual void setAttribute(Document::Attribute attr, const QVariant& val) = 0;
protected:
	DocumentProcessor(DocumentProcessorPrivate* dd);
private:
	Q_DECLARE_PRIVATE(DocumentProcessor)
protected:
	DocumentProcessorPrivate* d_ptr;
};

class XHtml11StreamWriterPrivate;
class XHtml11StreamWriter : public DocumentProcessor {
public:
	XHtml11StreamWriter(QIODevice* device = 0);
public:
	QXmlStreamWriter* xmlWriter();
	void setDevice(QIODevice* device);
public:
	void nextLinksTo(const QString& url);
	void writeStartDocument(const DocumentHead &head = DocumentHead());
	void writeEndDocument();
	void writeStartElement(Document::Tag special);
	void writeEndElement();
	void writeCharacters(const QString& str);
	void setAttribute(Document::Attribute attr, const QVariant& val);
private:
	Q_DECLARE_PRIVATE(XHtml11StreamWriter)
};

}

#endif
