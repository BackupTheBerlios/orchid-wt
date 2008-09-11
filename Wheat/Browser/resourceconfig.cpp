#include "resourceconfig.h"

#include <stem/resource.h>
#include <QtCore/QtDebug> // TODO rms

#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QLineEdit>

using namespace Orchid;
using namespace Orchid::Resource;
typedef IConfigurable::Option Option;

ResourceConfig::ResourceConfig(IResource *resource, QWidget* parent) : QDialog(parent) {
	m_resource = cast<IConfigurable*>(resource);
	m_advanced = cast<IAdvancedConfigurable*>(resource);
	m_options = m_resource->optionList();
	m_widgets.resize(m_options.size());
	
	setupUi(this);
	setupConfig();
}

#define GET_PROP(name) \
	(m_advanced && (value = m_advanced->optionProperty(option.first, name)).isValid())

void ResourceConfig::setupConfig() {
	if(!m_resource) return;
	QVariant value;

	QGridLayout* layout = new QGridLayout(configArea);

	int row = 0;
	foreach(Option option, m_options) {
		QString display = option.first;
		if(GET_PROP("display-name")) display = value.toString();		
		layout->addWidget(new QLabel(display), row, 0);
		
		if(option.second == qMetaTypeId<int>()) {
			QSpinBox* spinBox = new QSpinBox();
			if(GET_PROP("min")) spinBox->setMinimum(value.toInt());
			else spinBox->setMinimum(-32768);
			if(GET_PROP("max")) spinBox->setMaximum(value.toInt());
			else spinBox->setMaximum(32767);
			spinBox->setValue(m_resource->option(option.first).toInt());
			layout->addWidget(spinBox, row, 1);
			m_widgets[row] = spinBox;
		} else if(option.second == qMetaTypeId<QString>()) {
			QLineEdit* lineEdit = new QLineEdit(m_resource->option(option.first).toString());
			layout->addWidget(lineEdit, row, 1);
			m_widgets[row] = lineEdit;
		} else {
			layout->addWidget(new QLabel("< Option-type not supportet >"), row, 1);
			m_widgets[row] = 0;
		}
		++row;
	}
	layout->setRowStretch(row, 1);
}

void ResourceConfig::accept() {
	int row = 0;
	foreach(Option option, m_options) {
		if(option.second == qMetaTypeId<int>()) {
			QSpinBox* spinBox = static_cast<QSpinBox*>(m_widgets[row]);
			m_resource->setOption(option.first, spinBox->value());
		} else if(option.second == qMetaTypeId<QString>()) {
			QLineEdit* lineEdit = static_cast<QLineEdit*>(m_widgets[row]);
			m_resource->setOption(option.first, lineEdit->text());
		}
		++row;
	}

	QDialog::accept();
}
