#include "newresourcedialog.h"

#include <stem/resourcefactory.h>

// TODO make ok only pressable when name and type are selected

NewResourceDialog::NewResourceDialog(QWidget *parent) : QDialog(parent) {
	setupUi(this);
	
	QStringList keys = Orchid::ResourceFactory::keys();
	keys.sort();
	listWidget->addItems(keys);
}

QString NewResourceDialog::resourceType() const {
	return listWidget->currentItem()->text();
}

QString NewResourceDialog::resourceName() const {
	return nameEdit->text();
}
