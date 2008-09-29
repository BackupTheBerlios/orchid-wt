#include "configurationloader.h"

#include "resourcefactory.h"
#include "resourcekeep.h"
#include "resource.h"

#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtXml/QXmlStreamReader>

#include <QtCore/QtDebug>

using namespace Orchid::Resource;

namespace Orchid {

/**
 * \class ConfigurationLoader
 *
 * \brief ConfigurationLoader provides an interface for loading
 * of resource trees from configuration files.
 *
 * \sa ConfigurationWriter
 */

class ConfigurationLoaderPrivate {
	Q_DECLARE_PUBLIC(ConfigurationLoader);
public:
	ConfigurationLoaderPrivate(ConfigurationLoader *loader);
public:
	void readConfig(Handle *dest);
	void readElement(Handle *dest);
	void readOptions(Handle *handle);
protected:
	ConfigurationLoader *q_ptr;
private:
	QString fileName;
	QIODevice *device;
	QXmlStreamReader *xml;
};

ConfigurationLoaderPrivate::ConfigurationLoaderPrivate(ConfigurationLoader *loader) {
	q_ptr = loader;
}

void ConfigurationLoaderPrivate::readConfig(Handle *dest) {
	while(!xml->atEnd()) {
		xml->readNext();
		if(xml->isStartElement()) {
			if(xml->name() == "element") {
				readElement(dest);
			} else {
				xml->raiseError(QString("'%1' is not allowed here.").arg(xml->name().toString()));
			}
		} else if(xml->isEndElement()) {
			break;
		}
	}
}

void ConfigurationLoaderPrivate::readElement(Handle *dest) {
	QXmlStreamAttributes attrs = xml->attributes();
	QString name = attrs.value("name").toString();
	QString type = attrs.value("type").toString();
	Base *res = ResourceFactory::create(type);
	if(!res) {
		xml->raiseError(QString("Could not create resource of type '%1'.").arg(type));
		return;
	}
	IContainer *container = cast<IContainer*>(dest->resource());
	if(!container) {
		xml->raiseError(QString("'%1' does not allow child resources.").arg(dest->name()));
		return;
	}
	if(!container->addResource(name, res)) {
		xml->raiseError(QString("Could not insert resource '%1' into '%2'.").arg(name).arg(dest->name()));
		return;
	}
	Handle handle = container->child(name);
	if(handle.isNull()) {
		xml->raiseError(QString("'%1' succesfully added to '%2' but not accessible afterwards").arg(name).arg(dest->name()));
		return;
	}
	while(!xml->atEnd()) {
		xml->readNext();
		if(xml->isStartElement()) {
			if(xml->name() == "option") {
				readOptions(&handle);
			} else if (xml->name() == "element") {
				readElement(&handle);
			} else {
				xml->raiseError(QString("'%1' is not allowed here.").arg(xml->name().toString()));
			}
		} else if(xml->isEndElement()) {
			break;
		}
	}
}

void ConfigurationLoaderPrivate::readOptions(Handle *handle) {
	QXmlStreamAttributes attrs = xml->attributes();
	QString name = attrs.value("name").toString();
	QString value = attrs.value("value").toString();
	IConfigurable *config = Resource::cast<IConfigurable*>(handle->resource());
	if(!config) {
		xml->raiseError(QString("'%1' does not allow options").arg(handle->name()));
		return;
	}
	QList<IConfigurable::Option> optionList = config->optionList();
	int type = 0;
	foreach(IConfigurable::Option option, optionList) {
		if(option.first == name) {
			type = option.second;
			break;
		}
	}
	if(!type) {
		xml->raiseError(QString("'%1' is no valid option for '%2'").arg(name).arg(handle->name()));
		return;
	}
	
	// Convert sting value back into a variant
	QVariant variant;
	if(type == qMetaTypeId<QString>()) {
		variant = value;
	} else if(type == qMetaTypeId<int>()) {
		variant = value.toInt();
	} else if(type == qMetaTypeId<QStringList>()) {
		if(value.isEmpty()) {
			variant = QStringList();
		} else {
			QStringList list = value.split(" ");
			list.replaceInStrings("\\_", " ");
			list.replaceInStrings("\\\\", "\\");
			variant = list;
		}
	} else {
		xml->raiseError(QString("Type of option '%1' in resource '%2' not supported.").arg(name).arg(handle->name()));
		return;
	}
	
	if(!config->setOption(name, variant)) {
		xml->raiseError(QString("Could not set option '%1' of '%2' to '%3'.").arg(name).arg(handle->name()).arg(value));
		return;
	}
	while(!xml->atEnd()) {
		xml->readNext();
		if(xml->isStartElement()) {
			xml->raiseError(QString("'%1' is not allowed here.").arg(xml->name().toString()));
		} else if(xml->isEndElement()) {
			break;
		}
	}
}

/**
 * Constructs a new configuration loader that load from \a fileName.
 */
ConfigurationLoader::ConfigurationLoader(const QString &fileName) {
	d_ptr = new ConfigurationLoaderPrivate(this);
	d_ptr->fileName = fileName;
	d_ptr->device = new QFile(fileName);
	d_ptr->xml = new QXmlStreamReader(d_ptr->device);
}

/**
 * Construct a new configuration loader that load from \a device.
 */
ConfigurationLoader::ConfigurationLoader(QIODevice *device) {
	d_ptr = new ConfigurationLoaderPrivate(this);
	d_ptr->device = device;
	d_ptr->xml = new QXmlStreamReader(device);
}

/**
 * Construct a new configuration loader that loads from \a reader.
 */
ConfigurationLoader::ConfigurationLoader(QXmlStreamReader *reader) {
	d_ptr = new ConfigurationLoaderPrivate(this);
	d_ptr->xml = reader;
}

/**
 * Destructs the loader.
 */
ConfigurationLoader::~ConfigurationLoader() {
	if(d_ptr->device) {
		delete d_ptr->xml;
		d_ptr->xml = 0;
	}
	if(!d_ptr->fileName.isEmpty()) {
		delete d_ptr->device;
		d_ptr->device = 0;
	}
	delete d_ptr;
}

/**
 * Loads and returns a resource tree or returns Handle() on failure.
 */
Resource::Handle ConfigurationLoader::load() {
	Q_D(ConfigurationLoader);
	
	if(d->device && !d->device->isOpen() && !d->device->open(QIODevice::ReadOnly)) {
		return Handle();
	}
	
	Handle dest;
	dest.init(ResourceFactory::create("Container"));
	
	while(!d->xml->atEnd()) {
		d->xml->readNext();
		if(d->xml->isStartElement()) {
			if(d->xml->name() == "config") {
				d->readConfig(&dest);
			} else {
				d->xml->raiseError(QString("'%1' not allowed here").arg(d->xml->name().toString()));
			}
		} else if(d->xml->isEndElement()) {
			break;
		}
	}
	
	if(d->xml->error()) {
		qWarning() << "Load error: " << d->xml->errorString();
		dest = Handle();
	}
	return dest;
}

}
