#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

#include "ui_mainwindow.h"

#include <stem/resourcekeep.h>
#include <flower/style.h>
#include <flower/fragment.h>
#include <root/httpservice.h>
#include <QtNetwork/QHttp>
#include <QtCore/QBuffer>

namespace Orchid {
class HtmlStreamWriter;
class ResourceModel;
}

class MyStyle : public Orchid::Style {
	public:
		void writeHeading(Orchid::HtmlStreamWriter* writer, const QString& text) const;
		void setHeading(const QString& heading);
		QString content() const;
	private:
		QString m_headingStyle;
};

class MyFragment : public Orchid::Fragment {
	public:
		void build(Orchid::HtmlStreamWriter* writer);
	public:
		MyStyle* style;
};


class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
public:
	MainWindow();
public slots:
	void activateResource(const QModelIndex& index);
	void configResource();
private slots:
	void requestFinished(int id, bool error);
private:
	Orchid::ResourceModel* m_model;
	Orchid::Resource::Handle m_root;
	MyStyle m_style;
	MyFragment m_fragment;
	Orchid::HttpService m_service;
	QHttp reader;
	QBuffer result;
};

#endif
