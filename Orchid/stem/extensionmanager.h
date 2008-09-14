#ifndef _ORCHID_EXTIONSIONMANAGER_H_
#define _ORCHID_EXTIONSIONMANAGER_H_

#include <QtCore/QtGlobal>

class QString;
class QStringList;

namespace Orchid {

class FactoryHelper {
public:
	virtual ~FactoryHelper() {}
public:
	virtual QStringList keys() const = 0;
};

class ExtensionManager {
public:
	static QStringList keys(const QString &category);
	static FactoryHelper *helper(const QString &category, const QString &key);
};

}

#endif
