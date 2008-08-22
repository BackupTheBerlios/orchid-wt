#ifndef _ORCHID_XMLFRAGMENTREADER_H_
#define _ORCHID_XMLFRAGMENTREADER_H_

#include <QtCore/QtGlobal>

class QXmlStreamReader;

namespace Orchid {

class DomFragment;

class XmlFragmentReaderPrivate;
class XmlFragmentReader {
public:
	enum ErrorCode {
		NoError = 0,
		UnallowedElement,
	};
public:
	XmlFragmentReader(QXmlStreamReader* xml);
	~XmlFragmentReader();
public:
	DomFragment* read();
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
