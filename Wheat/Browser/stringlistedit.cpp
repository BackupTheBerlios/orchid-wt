#include "stringlistedit.h"

#include "stringlistedit.moc"

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
