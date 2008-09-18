#ifndef _ORCHID_DOCSTREAMSPLUGIN_H_
#define _ORCHID_DOCSTREAMSPLUGIN_H_

#include <stem/extensionplugin.h>

class DocStreamsPlugin : public Orchid::ExtensionPlugin {
	Q_OBJECT
public:
	DocStreamsPlugin();
	~DocStreamsPlugin();
public:
	QList<Orchid::FactoryHelper*> helpers() const;
private:
	QList<Orchid::FactoryHelper*> m_helpers;
};

#endif
