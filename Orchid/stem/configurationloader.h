#ifndef _ORCHID_CONFIGURATIONLOADER_H_
#define _ORCHID_CONFIGURATIONLOADER_H_

#include "globals.h"

#include <QtCore/QtGlobal>

class QIODevice;
class QXmlStreamReader;

namespace Orchid {

namespace Resource {
class Handle;
}

class ConfigurationLoaderPrivate;
class ORCHID_STEM_EXPORT ConfigurationLoader {
public:
	ConfigurationLoader(const QString &fileName);
	ConfigurationLoader(QIODevice *device);
	ConfigurationLoader(QXmlStreamReader *reader);
	~ConfigurationLoader();
public:
	Resource::Handle load();
protected:
	ConfigurationLoaderPrivate *d_ptr;
private:
	Q_DECLARE_PRIVATE(ConfigurationLoader)
};

}

#endif
