#include "mainwindow.h"

#include "mainwindow.moc"

#include <QBuffer>
#include <QtDebug>
#include <QXmlStreamWriter>

#include "htmlstreamwriter.h"

#include "document.h"

#include "style.h"
#include "styleattributes.h"

#include "resourcemodel.h"
#include "request.h"


class BambooResource : public Orchid::Resource::Resource, public Orchid::Resource::IQueryable {
public:
	void addStyle(Bamboo::Style* style);
	void setMainFragment(Bamboo::Fragment* fragment);
	void query(Orchid::Request* request);
private:
	Bamboo::Document m_doc;
};





void MyStyle::writeHeading(Bamboo::HtmlStreamWriter* writer, const QString& text) const {
	Bamboo::StyleAttributes attrs = writer->attributes(this);
	QXmlStreamWriter* xml = writer->xmlWriter();

	xml->writeStartElement("h1");
	QString classname = attrs.classname("heading1");
	if(!classname.isNull()) {
		xml->writeAttribute("class", classname);
	} else if(!m_headingStyle.isEmpty()) {
		xml->writeAttribute("style", m_headingStyle);
	}
	xml->writeCharacters(text);
	xml->writeEndElement();
}

void MyStyle::setHeading(const QString& heading) {
	m_headingStyle = heading;
}

QString MyStyle::content() const {
	return QString(".heading1 { ") + m_headingStyle + "}";
}



void MyFragment::build(Bamboo::HtmlStreamWriter* writer) {
	QXmlStreamWriter* xml = writer->xmlWriter();
	style->writeHeading(writer, "Test");
	xml->writeTextElement("p", "This is a very simple Test-Page");
}

void BambooResource::addStyle(Bamboo::Style* style) {
	m_doc.addGlobalStyle(style);
}

void BambooResource::setMainFragment(Bamboo::Fragment* fragment) {
	m_doc.setMainFragment(fragment);
}

void BambooResource::query(Orchid::Request* request) {
	if(request->method() != Orchid::HttpGetMethod) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;
	
	Bamboo::HtmlStreamWriter writer;
	writer.setDevice(request);
	m_doc.build(&writer);
	
}

MainWindow::MainWindow() : m_service(8000) {
	setupUi(this);


	Bamboo::HtmlStreamWriter writer;
	QBuffer buf;
	buf.open(QIODevice::WriteOnly);
	
	writer.setDevice(&buf);


	m_style.setHeading("background-color: red");

	
	m_fragment.style = &m_style;
	Bamboo::Document doc;
	doc.addGlobalStyle(&m_style);
	doc.setMainFragment(&m_fragment);
	doc.build(&writer);
	
	BambooResource *sample = new BambooResource();
	sample->addStyle(&m_style);
	sample->setMainFragment(&m_fragment);

	Orchid::SimpleTextResource *text = new Orchid::SimpleTextResource("Hello World");

	// Create sample resources
	Orchid::Resource::Resource *t1 = new Orchid::Resource::Resource();
	Orchid::Resource::Resource *t2 = new Orchid::Resource::Resource();
	Orchid::Resource::Resource *t3 = new Orchid::Resource::Resource();
	Orchid::Resource::Resource *t4 = new Orchid::Resource::Resource();
	Orchid::ModelResource *res2 = new Orchid::ModelResource();
	Orchid::ModelResource *res = new Orchid::ModelResource();


	res2->addResource("sample.html", sample);
	res2->addResource("text.txt", text);
	res2->addResource("test1", t1);
	res2->addResource("test2", t2),
	res->addResource("test3", t3);
	res->addResource("dir", res2);
	res->addResource("test4", t4);

	m_root.init(res);

	
	m_service.setRoot(m_root);


	qDebug() << "located: " << Orchid::Resource::Resource::locateUrl(m_root, QUrl("dir/sample.html")).name();
	
	treeView->setModel(new Orchid::ResourceModel(res, this));
	
	webView->setUrl(QUrl("http://localhost:8000/dir/sample.html?1"));
// 	webView->setContent(buf.data());
	sourceView->setPlainText(buf.data());
	

}
