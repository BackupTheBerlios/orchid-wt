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
	void addResource();
	void configResource();
private slots:
	void requestFinished(int id, bool error);
	void fileNew();
	void fileOpen();
	void fileSave();
	void fileSaveAs();
	void about();
	void aboutOrchid();
private:
	void loadPlugins();
	void setupActions();
	void setupExamples();
private:
	Orchid::ResourceModel* m_model;
	Orchid::Resource::Handle m_root;
	Orchid::HttpService m_service;
	QHttp reader;
	QBuffer result;

	QString m_currentFile;
};

#endif
