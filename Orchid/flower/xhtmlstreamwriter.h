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
	void startDocument(const DocumentHead &head = DocumentHead());
	void endDocument();
	void startElement(Document::Tag special);
	void endElement();
	void insertCharacters(const QString& str);
	void setAttribute(Document::Attribute attr, const QVariant& val);
private:
	Q_DECLARE_PRIVATE(XHtml11StreamWriter)
};

}

#endif
