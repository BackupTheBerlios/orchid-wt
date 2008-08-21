#ifndef _ORCHID_XMLFRAGMENTWRITER_H_
#define _ORCHID_XMLFRAGMENTWRITER_H_

#include <QtCore/QtGlobal>

class QXmlStreamWriter;

namespace Orchid {

class DomFragment;

class XmlFragmentWriterPrivate;
class XmlFragmentWriter {
public:
	XmlFragmentWriter();
	~XmlFragmentWriter();
public:
	bool write(QXmlStreamWriter* xml, DomFragment* fragment);
protected:
	XmlFragmentWriterPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(XmlFragmentWriter)
};

}

#endif
