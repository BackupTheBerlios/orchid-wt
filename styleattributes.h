#ifndef _STYLEATTRIBUTES_H_
#define _STYLEATTRIBUTES_H_

#include <QSharedData>
#include <QString>

namespace Bamboo {

class StyleAttributes;

class StyleAttributesData : public QSharedData {
	QString prefix;
	friend class StyleAttributes;
};

class StyleAttributes {
public:
	StyleAttributes();
	StyleAttributes(const QString& prefix);
public:
	QString classname(const QString& name);
private:
	QSharedDataPointer<StyleAttributesData> d;
};

}

#endif 
