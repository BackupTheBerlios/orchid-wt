#ifndef _BAMBOO_BUILDER_H_
#define _BAMBOO_BUILDER_H_

#include "obuilder.h"

class QXmlStreamWriter;

namespace Bamboo {

class BuilderPrivate;

class Builder : public Orchid::Builder {
public:
	Builder();
public:
	QXmlStreamWriter* xml();
	void setDevice(QIODevice* device);
private:
	Q_DECLARE_PRIVATE(Builder)
};

}

#endif
