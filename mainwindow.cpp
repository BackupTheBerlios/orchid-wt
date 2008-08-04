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
	writer->writeBeginSpecial(Bamboo::HtmlSpecialSection);
	writer->writeBeginSpecial(Bamboo::HtmlSpecialSection);
	writer->writeSimpleSpecial(Bamboo::HtmlSpecialHeading, "Test");
	writer->writeEndSpecial();
	writer->writeEndSpecial();
// 	style->writeHeading(writer, "Test");
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
	
	Bamboo::XHtml11StreamWriter writer;
	writer.setDevice(request);
	m_doc.build(&writer);
	
}

MainWindow::MainWindow() : m_service(8000) {
	setupUi(this);

	m_style.setHeading("background-color: red");
	m_fragment.style = &m_style;
	
	BambooResource *sample = new BambooResource();
	sample->addStyle(&m_style);
	sample->setMainFragment(&m_fragment);

	Orchid::SimpleTextResource *text = new Orchid::SimpleTextResource("Hello World");

	// Create sample resources
	Orchid::Resource::Resource *t1 = new Orchid::Resource::Resource();
	Orchid::Resource::Resource *t2 = new Orchid::Resource::Resource();
	Orchid::ModelResource *dir = new Orchid::ModelResource();
	Orchid::ModelResource *res = new Orchid::ModelResource();

	dir->addResource("sample.html", sample);
	dir->addResource("text.txt", text);
	res->addResource("dir", dir);
	res->addResource("test1", t1);
	res->addResource("test2", t2),

	m_root.init(res);
	
	m_service.setRoot(m_root);

	treeView->setModel(new Orchid::ResourceModel(res, this));	
	webView->setUrl(QUrl("http://localhost:8000/dir/sample.html?1"));
	
	// TODO load sourceView from local server
// 	sourceView->setPlainText(buf.data());
}
