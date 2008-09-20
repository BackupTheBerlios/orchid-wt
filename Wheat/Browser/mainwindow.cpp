#include "mainwindow.h"

#include "mainwindow.moc"

#include <QtDebug>

#include <stem/resourcemodel.h>
#include <stem/location.h>

#include <stem/resourcefactory.h>

#include "newresourcedialog.h"
#include "resourceconfig.h"

using namespace Orchid;
using namespace Orchid::Resource;

MainWindow::MainWindow() : m_service(8000) {
	setupUi(this);
	
	ContainerResource *res = static_cast<ContainerResource*>(ResourceFactory::create("Container"));
	
	ExtensionManager::loadExtension("Orchid/leaf/libimageplugin.so");
	ExtensionManager::loadExtension("Orchid/leaf/libmodelplugin.so");
	ExtensionManager::loadExtension("Examples/Gallery/libgalleryplugin.so");
	ExtensionManager::loadExtension("Examples/DocStreams/libdocstreamsplugin.so");
	
	m_root.init(res);
	m_service.setRoot(m_root);
	m_model = new ResourceModel(m_root, this);
	
	res->addResource("sample.html", ResourceFactory::create("Document-Streams-Sample"));

	Base *xmlres = ResourceFactory::create("XmlModel");
	IConfigurable *config = cast<IConfigurable*>(xmlres);
	config->setOption("model", qVariantFromValue<QObject*>(m_model));
	res->addResource("resource.model", xmlres);
	
	Base *imgres = ResourceFactory::create("Image");
	config = cast<IConfigurable*>(imgres);
	config->setOption("path", "test.jpg");
	res->addResource("image.jpg", imgres);
	
	res->addResource("testcontainer", ResourceFactory::create("Container"));
	
	Base *demo = ResourceFactory::create("Hardcoded-Gallery-Demo");
	res->addResource("demo", demo);

	treeView->setModel(m_model);
	connect(treeView, SIGNAL(activated(const QModelIndex&)), this, SLOT(activateResource(const QModelIndex&)));

	connect(&reader, SIGNAL(requestFinished(int,bool)), this, SLOT(requestFinished(int,bool)));
	reader.setHost("localhost", 8000);

	setupActions();
}

void MainWindow::setupActions() {
	connect(actionNew, SIGNAL(triggered()), this, SLOT(fileNew()));
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	connect(addResourceButton, SIGNAL(clicked(bool)), this, SLOT(addResource()));
	connect(configResourceButton, SIGNAL(clicked(bool)), this, SLOT(configResource()));
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
	qDebug() << "fileOpen";
}

void MainWindow::fileSave() {
	qDebug() << "fileSave";
}

void MainWindow::fileSaveAs() {
	qDebug() << "fileSaveAs";
}
