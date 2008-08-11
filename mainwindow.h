#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

#include "resourcekeep.h"

#include "ui_mainwindow.h"

#include "style.h"
#include "fragment.h"
#include "httpservice.h"

namespace Bamboo {
class HtmlStreamWriter;
}

namespace Orchid {
class ResourceModel;
}

class MyStyle : public Bamboo::Style {
	public:
		void writeHeading(Bamboo::HtmlStreamWriter* writer, const QString& text) const;
		void setHeading(const QString& heading);
		QString content() const;
	private:
		QString m_headingStyle;
};

class MyFragment : public Bamboo::Fragment {
	public:
		void build(Bamboo::HtmlStreamWriter* writer);
	public:
		MyStyle* style;
};


class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
public:
	MainWindow();
public slots:
	void activateResource(const QModelIndex& index);
private:
	Orchid::ResourceModel* m_model;
	Orchid::Resource::Handle m_root;
	MyStyle m_style;
	MyFragment m_fragment;
	Orchid::HttpService m_service;
};

#endif
