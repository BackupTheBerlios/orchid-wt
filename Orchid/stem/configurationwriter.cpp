#include "configurationwriter.h"

#include "resourcekeep.h"
#include "resource.h"

#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtXml/QXmlStreamWriter>

#include <QtCore/QtDebug>

using namespace Orchid::Resource;

namespace Orchid {

/**
 * \class ConfigurationWriter
 *
 * \brief ConfigurationWriter provides an interface for
 * stroring resource trees to configuration files.
 *
 * \sa ConfigurationLoader
 */

class ConfigurationWriterPrivate {
	Q_DECLARE_PUBLIC(ConfigurationWriter);
public:
	ConfigurationWriterPrivate(ConfigurationWriter *writer);
public:
	void writeConfig(const Handle &handle);
	void writeElement(const Handle &handle);
protected:
	ConfigurationWriter *q_ptr;
private:
	QString fileName;
	QIODevice *device;
	QXmlStreamWriter *xml;
};

ConfigurationWriterPrivate::ConfigurationWriterPrivate(ConfigurationWriter *writer) {
	q_ptr = writer;
}

void ConfigurationWriterPrivate::writeConfig(const Handle &handle) {
	IConfigurable *config = Resource::cast<IConfigurable*>(handle.resource());
	if(config) {
		QList<IConfigurable::Option> options = config->optionList();
		foreach(IConfigurable::Option option, options) {
			int type = option.second;
			QString value;
			QVariant variant = config->option(option.first);
			
			// Convert variant into a string
			if(type == qMetaTypeId<QString>()) {
				value = variant.toString();
			} else if(type == qMetaTypeId<int>()) {
				value = variant.toString();
			} else if(type == qMetaTypeId<QStringList>()) {
				// TODO change this
				QStringList list = variant.toStringList();
				if(list.isEmpty()) {
					value = "";
				} else {
					list.replaceInStrings("\\", "\\\\");
					list.replaceInStrings(" ", "\\_");
					value = list.join(" ");
				}
			}
			
			if(value.isNull()) {
				xml->writeComment(QString("Type of option '%1' not supported.").arg(option.first));
			} else {
				xml->writeEmptyElement("option");
				xml->writeAttribute("name", option.first);
				xml->writeAttribute("value", value);
			}
		}
	}
}
void ConfigurationWriterPrivate::writeElement(const Handle &handle) {
	xml->writeStartElement("element");
	xml->writeAttribute("name", handle.name());
	xml->writeAttribute("type", handle.resource()->typeName());
	writeConfig(handle);
	IContainer *container = Resource::cast<IContainer*>(handle.resource());
	if(container) {
		QStringList childs = container->childs();
		foreach(QString name, childs) {
			Handle child(container->child(name));
			if(child.isNull()) continue;
			writeElement(child);
		}
	}
	xml->writeEndElement();
}



/**
 * Constructs a new configuration writer that writes to \a fileName.
 */
ConfigurationWriter::ConfigurationWriter(const QString &fileName) {
	d_ptr = new ConfigurationWriterPrivate(this);
	d_ptr->fileName = fileName;
	d_ptr->device = new QFile(fileName);
	d_ptr->xml = new QXmlStreamWriter(d_ptr->device);
}

/**
 * Constructs a new configuration writer that writes to \a device.
 */
ConfigurationWriter::ConfigurationWriter(QIODevice *device) {
	d_ptr = new ConfigurationWriterPrivate(this);
	d_ptr->device = device;
	d_ptr->xml = new QXmlStreamWriter(device);
}

/**
 * Constructs a new configuration writer that writes to \a writer.
 */
ConfigurationWriter::ConfigurationWriter(QXmlStreamWriter *writer) {
	d_ptr = new ConfigurationWriterPrivate(this);
	d_ptr->xml = writer;
}

/**
 * Destructs the writer.
 */
ConfigurationWriter::~ConfigurationWriter() {
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
 * Stores the resource tree at \a handle and returns true on
 * success or false on failure.
 */
bool ConfigurationWriter::store(const Handle &handle) {
	Q_D(ConfigurationWriter);
	if(d->device && !d->device->isOpen() && !d->device->open(QIODevice::WriteOnly)) {
		return false;
	}
	
	d->xml->setAutoFormatting(true);
	d->xml->writeStartDocument();
	d->xml->writeStartElement("config");
	
	IContainer *container = Resource::cast<IContainer*>(handle.resource());
	if(container) {
		QStringList childs = container->childs();
		foreach(QString name, childs) {
			Handle child(container->child(name));
			if(child.isNull()) continue;
			d->writeElement(child);
		}
	}

	d->xml->writeEndDocument();
	return true;
}


}
