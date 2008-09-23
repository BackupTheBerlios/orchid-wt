#ifndef _ORCHID_XMLFRAGMENTREADER_H_
#define _ORCHID_XMLFRAGMENTREADER_H_

#include <QtCore/QtGlobal>

class QXmlStreamReader;
class QIODevice;

namespace Orchid {

class DocumentProcessor;

class XmlFragmentReaderPrivate;
class XmlFragmentReader {
public:
	enum ErrorCode {
		NoError = 0,
		UnallowedElement,
	};
public:
	XmlFragmentReader();
	XmlFragmentReader(DocumentProcessor *dest, QXmlStreamReader* xml = 0);
	XmlFragmentReader(DocumentProcessor *dest, QIODevice *device);
	~XmlFragmentReader();
public:
	DocumentProcessor *processor() const;
	void setProcessor(DocumentProcessor *processor);
	QIODevice *device() const;
	void setDevice(QIODevice *device);
	QXmlStreamReader *xmlStreamReader() const;
	void setXmlStreamReader(QXmlStreamReader *xml);
	bool readBody();
	bool readDocument();
	ErrorCode errorCode() const;
	QString errorString() const;
	int errorLine() const;
	int errorColumn() const;
private:
	XmlFragmentReaderPrivate* d_ptr;
	Q_DECLARE_PRIVATE(XmlFragmentReader);
};

}

#endif
