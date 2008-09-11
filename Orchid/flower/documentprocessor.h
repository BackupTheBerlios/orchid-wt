#ifndef _ORCHID_DOCUMENTPROCESSOR_H_
#define _ORCHID_DOCUMENTPROCESSOR_H_

#include <QtGlobal>
#include "documenthead.h"
#include "documentenums.h"

class QString;
class QVariant;

namespace Orchid {
	
class StyleAttributes;
class Style;

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

}

#endif
