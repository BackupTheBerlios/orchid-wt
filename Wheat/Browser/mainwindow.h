#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

#include "ui_mainwindow.h"

#include <stem/resourcekeep.h>
#include <root/httpservice.h>
#include <QtNetwork/QHttp>
#include <QtCore/QBuffer>

namespace Orchid {
class ResourceModel;
}

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
	Orchid::HttpService m_service;
	QHttp reader;
	QBuffer result;
};

#endif
