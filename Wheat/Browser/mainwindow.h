#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

#include <stem/resourcekeep.h>

#include "ui_mainwindow.h"

#include <flower/style.h>
#include <flower/fragment.h>
#include <root/httpservice.h>
#include <QtNetwork/QHttp>
#include <QtCore/QBuffer>

namespace Orchid {
class DocumentProcessor;
class ResourceModel;
}

class MyStyle : public Orchid::Style {
	public:
		void writeHeading(Orchid::DocumentProcessor* writer, const QString& text) const;
		void setHeading(const QString& heading);
		QString content() const;
	private:
		QString m_headingStyle;
};

class MyFragment : public Orchid::Fragment {
	public:
		void build(Orchid::DocumentProcessor* writer);
	public:
		MyStyle* style;
};


class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
public:
	MainWindow();
public slots:
	void activateResource(const QModelIndex& index);
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
