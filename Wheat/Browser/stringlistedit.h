#ifndef _ORCHID_STRINGLISTEDIT_H_
#define _ORCHID_STRINGLISTEDIT_H_

#include <QWidget>

#include "ui_stringlistedit.h"

class StringListEdit : public QWidget, private Ui::StringListEdit {
	Q_OBJECT
public:
	StringListEdit();
public slots:
	void addNew();
	void remove();
public:
	void setList(const QStringList &list);
	QStringList list() const;
};

#endif
