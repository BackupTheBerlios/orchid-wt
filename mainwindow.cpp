#include "mainwindow.h"

#include "mainwindow.moc"

#include <QBuffer>
#include <QtDebug>
#include <QXmlStreamWriter>

#include "builder.h"

#include "document.h"
#include "dependency.h"

class MyFragment : public Bamboo::Fragment {
public:
	void getDependencies(Dependencies& deps) const;
	void build(Bamboo::Builder* builder);
};

void MyFragment::getDependencies(Dependencies& deps) const {
	deps.insert(Dependency::externalStylesheet("test.css", "screen"));
}

void MyFragment::build(Bamboo::Builder* builder) {
	QXmlStreamWriter* writer = builder->xml();
	writer->writeTextElement("h1", "Test");
	writer->writeTextElement("p", "This is a very simple Test-Page");
}

MainWindow::MainWindow() {
	setupUi(this);
	Bamboo::Builder builder;
	QBuffer buf;
	buf.open(QIODevice::WriteOnly);
	
	builder.setDevice(&buf);

	MyFragment fragment;
	Bamboo::Document doc;
	doc.setMainFragment(&fragment);
	doc.build(&builder);
	
	webView->setContent(buf.data());
	sourceView->setPlainText(buf.data());
}
