#ifndef _ORCHID_RESOURCECONFIG_H_
#define _ORCHID_RESOURCECONFIG_H_

#include <QDialog>

#include "ui_resourceconfig.h"
#include <stem/resource.h>

class ResourceConfig : public QDialog, private Ui::ResourceConfig {
public:
	ResourceConfig(Orchid::Resource::IResource* resource, QWidget* parent = 0);
public:
	void accept();
private:
	void setupConfig();
private:
	Orchid::Resource::IConfigurable *m_resource;
	Orchid::Resource::IAdvancedConfigurable *m_advanced;
	QList<Orchid::Resource::IConfigurable::Option> m_options;
	QVector<QWidget*> m_widgets;
};

#endif
