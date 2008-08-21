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
	XmlFragmentReader();
	~XmlFragmentReader();
public:
	DomFragment* read(QXmlStreamReader* xml);
	ErrorCode errorCode() const;
	QString errorString() const;
private:
	XmlFragmentReaderPrivate* d_ptr;
	Q_DECLARE_PRIVATE(XmlFragmentReader);
};

}

#endif
