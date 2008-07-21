#ifndef _BAMBOO_BUILDER_H_
#define _BAMBOO_BUILDER_H_

#include "obuilder.h"

class QXmlStreamWriter;

namespace Bamboo {
	
class StyleAttributes;
class Style;

class BuilderPrivate;

class Builder : public Orchid::Builder {
public:
	Builder();
	Builder(QIODevice* device);
public:
	QXmlStreamWriter* xml();
	void setDevice(QIODevice* device);
public:
	StyleAttributes attributes(const Style* style);
	void regStyle(const Style* style, const QString& prefix);
private:
	Q_DECLARE_PRIVATE(Builder)
};

}

#endif
