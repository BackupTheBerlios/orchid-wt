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


#include "stringlistedit.h"

StringListEdit::StringListEdit() {
	setupUi(this);

	connect(addButton, SIGNAL(clicked()), this, SLOT(addNew()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(remove()));
}

void StringListEdit::setList(const QStringList &list) {
	QStringList::const_iterator it;
	for(it = list.begin(); it != list.end(); ++it) {
		QListWidgetItem *item = new QListWidgetItem();
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setText(*it);
		
		listWidget->addItem(item);
	}

    listWidget->setCurrentRow(0);
}

QStringList StringListEdit::list() const {
	QStringList list;
	int count = listWidget->count();
	for(int i = 0; i < count; ++i) {
		list.append(listWidget->item(i)->text());
	}
	return list;
}

void StringListEdit::addNew() {
	int pos = listWidget->currentRow() + 1;
	
	QListWidgetItem *item = new QListWidgetItem();
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setText("- add text here -");
	listWidget->insertItem(pos, item);
	
	listWidget->setCurrentItem(item);
	listWidget->editItem(item);
}

void StringListEdit::remove() {
	int pos = listWidget->currentRow();
	if(pos < 0) return;
	
	delete listWidget->takeItem(pos);
}
