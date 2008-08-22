#ifndef _ORCHID_HTMLFRAGMENTWRITER_H_
#define _ORCHID_HTMLFRAGMENTWRITER_H_

#include <QtCore/QtGlobal>

namespace Orchid {

class HtmlStreamWriter;
class DomFragment;

class HtmlFragmentWriterPrivate;
class HtmlFragmentWriter {
public:
	HtmlFragmentWriter(HtmlStreamWriter* writer);
	~HtmlFragmentWriter();
public:
	void write(DomFragment* fragment);
protected:
	HtmlFragmentWriterPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(HtmlFragmentWriter)
};

}

#endif
