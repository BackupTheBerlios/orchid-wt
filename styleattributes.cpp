#include "styleattributes.h" 

#include <QString>

namespace Bamboo {

StyleAttributes::StyleAttributes() {
	d = new StyleAttributesData;
}

StyleAttributes::StyleAttributes(const QString& prefix) {
	d = new StyleAttributesData;
	d->prefix = prefix;
}

QString StyleAttributes::classname(const QString& classname) {
	if(d->prefix.isNull()) return QString();
	return d->prefix+classname;
}

}
