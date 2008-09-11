#ifndef _ORCHID_XHTMLSTREAMWRITER_H_
#define _ORCHID_XHTMLSTREAMWRITER_H_

#include "documentprocessor.h"

class QXmlStreamWriter;
class QIODevice;

namespace Orchid {

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
