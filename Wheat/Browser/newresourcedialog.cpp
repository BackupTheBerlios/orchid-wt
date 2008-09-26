/*
 * This file is part of the Orchid tools applications
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


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
