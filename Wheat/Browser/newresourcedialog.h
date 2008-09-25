#ifndef _ORCHID_NEWRESOURCEDIALOG_H_
#define _ORCHID_NEWRESOURCEDIALOG_H_

#include <QDialog>

#include "ui_newresourcedialog.h"

class NewResourceDialog : public QDialog, private Ui::NewResourceDialog {
public:
	NewResourceDialog(QWidget *parent = 0);
public:
	QString resourceType() const;
	QString resourceName() const;
};

#endif
