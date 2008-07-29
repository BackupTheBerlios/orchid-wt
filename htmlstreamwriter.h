#ifndef _BAMBOO_BUILDER_H_
#define _BAMBOO_BUILDER_H_

#include <QtGlobal>

class QXmlStreamWriter;
class QIODevice;
class QString;

namespace Bamboo {
	
class StyleAttributes;
class Style;

class HtmlStreamWriterPrivate;

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
private:
	Q_DECLARE_PRIVATE(HtmlStreamWriter)
protected:
	HtmlStreamWriterPrivate* d_ptr;
};

}

#endif
