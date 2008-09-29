#ifndef _ORCHID_CONFIGURATIONWRITER_H_
#define _ORCHID_CONFIGURATIONWRITER_H_

#include "globals.h"

#include <QtCore/QtGlobal>

class QIODevice;
class QXmlStreamWriter;

namespace Orchid {
	
namespace Resource {
class Handle;
}

class ConfigurationWriterPrivate;
class ORCHID_STEM_EXPORT ConfigurationWriter {
	Q_DECLARE_PRIVATE(ConfigurationWriter)
public:
	ConfigurationWriter(const QString &fileName);
	ConfigurationWriter(QIODevice *device);
	ConfigurationWriter(QXmlStreamWriter *writer);
	~ConfigurationWriter();
public:
	bool store(const Resource::Handle &handle);
protected:
	ConfigurationWriterPrivate *d_ptr;
};

}

#endif
