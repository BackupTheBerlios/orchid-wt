#include "mainwindow.h"

#include "mainwindow.moc"

#include <QtDebug>

#include <stem/resourcemodel.h>
#include <stem/location.h>

#include <leaf/scriptedresource.h>

#include <QFile>

#include <stem/resourcefactory.h>

#include "resourceconfig.h"

MainWindow::MainWindow() : m_service(8000) {
	using namespace Orchid;
	
	setupUi(this);
	
	ContainerResource *res = static_cast<ContainerResource*>(ResourceFactory::create("Container"));
	
	ExtensionManager::loadExtension("Orchid/leaf/libimageplugin.so");
	ExtensionManager::loadExtension("Orchid/leaf/libmodelplugin.so");
	ExtensionManager::loadExtension("Examples/Gallery/libgalleryplugin.so");
	ExtensionManager::loadExtension("Examples/DocStreams/libdocstreamsplugin.so");
	
	m_root.init(res);
	m_service.setRoot(m_root);
	m_model = new Orchid::ResourceModel(res, this);
	
	res->addResource("sample.html", ResourceFactory::create("Document-Streams-Sample"));

	Resource::IResource *xmlres = ResourceFactory::create("XmlModel");
	Resource::IConfigurable *config = Resource::cast<Resource::IConfigurable*>(xmlres);
	config->setOption("model", qVariantFromValue<QObject*>(m_model));
	res->addResource("resource.model", xmlres);
	
	QFile scriptFile(":/test.js");
	scriptFile.open(QIODevice::ReadOnly);
	QString program(scriptFile.readAll());
	scriptFile.close();
	
	res->addResource("scripted", new Orchid::ScriptedResource(program, "MyResource"));

	Resource::IResource *demo = ResourceFactory::create("GalleryDemo");
	res->addResource("demo", demo);	

	treeView->setModel(m_model);
	connect(treeView, SIGNAL(activated(const QModelIndex&)), this, SLOT(activateResource(const QModelIndex&)));

	connect(configButton, SIGNAL(clicked(bool)), this, SLOT(configResource()));
	
	connect(&reader, SIGNAL(requestFinished(int,bool)), this, SLOT(requestFinished(int,bool)));
	reader.setHost("localhost", 8000);
}

void MainWindow::activateResource(const QModelIndex& index) {
	QString path = m_model->path(index);
	reader.get(path, &result);
}

void MainWindow::configResource() {
	using namespace Orchid::Resource;
	QModelIndex index = treeView->currentIndex();
	Handle handle(m_model->resource(index));
	
	if(!cast<IConfigurable*>(handle.resource())) return;
	
	ResourceConfig config(handle.resource(), this);
	if(config.exec() == QDialog::Accepted) {
		QString path = m_model->path(index);
		reader.get(path, &result);
	}
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
