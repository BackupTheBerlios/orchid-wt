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


#include "mainwindow.h"

#include "mainwindow.moc"

#include <QtDebug>

#include <stem/resourcemodel.h>
#include <stem/location.h>

#include <leaf/scriptedresource.h>

#include <QFile>
#include <QtGui/QMessageBox>

#include <stem/resourcefactory.h>
#include <stem/configurationloader.h>
#include <stem/configurationwriter.h>

#include "newresourcedialog.h"
#include "resourceconfig.h"

#include <QFileDialog>

using namespace Orchid;
using namespace Orchid::Resource;

MainWindow::MainWindow() : m_service(8000) {
	setupUi(this);
	
	loadPlugins();
	
	ContainerResource *res = static_cast<ContainerResource*>(ResourceFactory::create("Container"));
	
	m_root.init(res);
	m_service.setRoot(m_root);
	m_model = new ResourceModel(m_root, this);

	treeView->setModel(m_model);
	connect(treeView, SIGNAL(activated(const QModelIndex&)), this, SLOT(activateResource(const QModelIndex&)));

	connect(&reader, SIGNAL(requestFinished(int,bool)), this, SLOT(requestFinished(int,bool)));
	reader.setHost("localhost", 8000);

	setupActions();
	
	setupExamples();
}

void MainWindow::loadPlugins() {
	QDir pluginsDir = QDir(qApp->applicationDirPath());

	pluginsDir.cdUp();

#ifdef ORCHID_LOAD_LOCAL_PLUGINS
	// This code can be used to load the uninstalled plugins for testing
	QDir local(pluginsDir);
	local.cdUp();
	QString localPath = local.path();
	qDebug() << localPath;
#ifdef Q_OS_WIN
	ExtensionManager::loadExtension(localPath+"/Orchid/leaf/libimageplugin.dll");
	ExtensionManager::loadExtension(localPath+"/Orchid/leaf/libmodelplugin.dll");
	ExtensionManager::loadExtension(localPath+"/Examples/Gallery/libgalleryplugin.dll");
	ExtensionManager::loadExtension(localPath+"/Examples/DocStreams/libdocstreamsplugin.dll");
	ExtensionManager::loadExtension(localPath+"/Examples/I18N/libi18ndocplugin.dll");
#else
	ExtensionManager::loadExtension(localPath+"/Orchid/leaf/libimageplugin.so");
	ExtensionManager::loadExtension(localPath+"/Orchid/leaf/libmodelplugin.so");
	ExtensionManager::loadExtension(localPath+"/Examples/Gallery/libgalleryplugin.so");
	ExtensionManager::loadExtension(localPath+"/Examples/DocStreams/libdocstreamsplugin.so");
	ExtensionManager::loadExtension(localPath+"/Examples/I18N/libi18ndocplugin.so");
#endif
#endif

    pluginsDir.cd("share/Orchid/plugins");

    foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
		QString path = pluginsDir.absoluteFilePath(fileName);
		ExtensionManager::loadExtension(path);
    }
}

void MainWindow::setupActions() {
	connect(actionNew, SIGNAL(triggered()), this, SLOT(fileNew()));
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(actionAboutOrchid, SIGNAL(triggered()), this, SLOT(aboutOrchid()));
	connect(actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(addResourceButton, SIGNAL(clicked(bool)), this, SLOT(addResource()));
	connect(configResourceButton, SIGNAL(clicked(bool)), this, SLOT(configResource()));
}

void MainWindow::setupExamples() {
	QStringList availableResources(ResourceFactory::keys());
	IContainer *container = Resource::cast<IContainer*>(m_root.resource());
	
	if(availableResources.contains("Document-Streams-Sample")) {
		container->addResource("sample.html", ResourceFactory::create("Document-Streams-Sample"));
	}
	
	if(availableResources.contains("I18N-Document-Sample")) {
		container->addResource("i18n.html", ResourceFactory::create("I18N-Document-Sample"));
	}

	if(availableResources.contains("XmlModel")) {
		Base *xmlres = ResourceFactory::create("XmlModel");
		IConfigurable *config = cast<IConfigurable*>(xmlres);
		config->setOption("model", qVariantFromValue<QObject*>(m_model));
		container->addResource("resource.model", xmlres);
	}
	
	if(availableResources.contains("Hardcoded-Gallery-Demo")) {
		Base *demo = ResourceFactory::create("Hardcoded-Gallery-Demo");
		container->addResource("demo", demo);
	}

	if(availableResources.contains("Image")) {
		Base *imgres = ResourceFactory::create("Image");
		IConfigurable *config = cast<IConfigurable*>(imgres);
		config->setOption("path", ":/flower2.jpg");
		container->addResource("image.jpg", imgres);
	}

// 	if(availableResources.contains("Scripted-Resource")) {
		QFile scriptFile(":/test.js");
		scriptFile.open(QIODevice::ReadOnly);
		QString program(scriptFile.readAll());
		scriptFile.close();

		container->addResource("scripted", new Orchid::ScriptedResource(program, "MyResource"));
// 	}

	m_model->update();
}

void MainWindow::activateResource(const QModelIndex& index) {
	m_model->update(index);
	
	Handle handle(m_model->resource(index));
	Base *resource = handle.resource();
	
	if(cast<IConfigurable*>(resource)) {
		configResourceButton->setEnabled(true);
	} else {
		configResourceButton->setEnabled(false);
	}
	
	if(cast<IContainer*>(resource)) {
		addResourceButton->setEnabled(true);
	} else {
		addResourceButton->setEnabled(false);
	}
	
	QString path = m_model->path(index);
	reader.get(path, &result);
}

void MainWindow::addResource() {
	QModelIndex index = treeView->currentIndex();
	Handle handle(m_model->resource(index));
	
	IContainer *container = cast<IContainer*>(handle.resource());
	if(!container) return;
	
	NewResourceDialog dialog(this);
	if(dialog.exec() == QDialog::Accepted) {
		QString name = dialog.resourceName();
		if(name.isEmpty()) return;
		Base *resource = ResourceFactory::create(dialog.resourceType());
		if(!resource) return;
		if(container->addResource(name, resource)) {
			m_model->update();
		} else {
			delete resource;
		}
	}
}

void MainWindow::configResource() {
	QModelIndex index = treeView->currentIndex();
	Handle handle(m_model->resource(index));
	
	if(!cast<IConfigurable*>(handle.resource())) return;
	
	ResourceConfig config(handle.resource(), this);
	if(config.exec() == QDialog::Accepted) {
		QString path = m_model->path(index);
		reader.get(path, &result);
	}
	m_model->update();
}

void MainWindow::requestFinished(int id, bool error) {
	QHttpResponseHeader header = reader.lastResponse();
	if(!error) {
		result.close();
		QByteArray content = result.data();
		sourceView->setPlainText(content);
		webView->setContent(content, header.contentType(), QUrl("http://localhost:8000/"));
		result.setData(QByteArray());
	} else {
		sourceView->setPlainText("\n\n\t\tError reading resource!\n\n"+reader.errorString());
		webView->setContent("<h1>Error</h1>Couldn't read resource"+reader.errorString().toUtf8());
	}
}

void MainWindow::fileNew() {
	qDebug() << "fileNew";
	
	IContainer *root = cast<IContainer*>(m_root.resource());
	Q_ASSERT(root);
	root->removeAll();
	m_model->update();
}

void MainWindow::fileOpen() {
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty()) {
		ConfigurationLoader loader(fileName);
		// TODO change this to load into a new root
		Handle newRoot = loader.load();
		if(!newRoot.isNull()) {
			m_currentFile = fileName;
			m_root = newRoot;
			m_service.setRoot(m_root);
			delete m_model;
			m_model = new ResourceModel(m_root, this);
			treeView->setModel(m_model);
		} else {
			qDebug() << "failed loading";
		}
		m_model->update();
	}
}

void MainWindow::fileSave() {
	if(m_currentFile.isEmpty()) {
		fileSaveAs();
	} else {
		ConfigurationWriter writer(m_currentFile);
		writer.store(m_root);
	}
}

void MainWindow::fileSaveAs() {
    QString fileName = QFileDialog::getSaveFileName(this);
    if(fileName.isEmpty()) return;

	ConfigurationWriter writer(fileName);
	if(writer.store(m_root)) {
		m_currentFile = fileName;
	}
}

void MainWindow::about() {
	QMessageBox::about(this, "About Orchid Browser",
		"<h1>Orchid Browser</h1>"
		"<p>The Orchid-Browser can be used to browse and configure "
		"Orchid-services.</p>"
		"<p>Copyright (C) 2008 Johannes Bergmeier</p>"
		"<p>This program is free software: you can redistribute it "
		"and/or modify it under the terms of the GNU General Public "
		"License version 3 as published by the Free Software "
		"Foundation.</p>"
		"<p>This program is distributed in the hope that it will be "
		"useful, but WITHOUT ANY WARRANTY; without even the implied "
		"warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR "
		"PURPOSE.  See the GNU General Public License for more "
		"details.</p>"
		"<p>You should have received a copy of the GNU General Public "
		"License along with this program.  If not, see "
		"<a href=\"http://www.gnu.org/licenses/\">"
		"http://www.gnu.org/licenses/</a>.</p>");
}

void MainWindow::aboutOrchid() {
	QMessageBox::about(this, "", "");
}
