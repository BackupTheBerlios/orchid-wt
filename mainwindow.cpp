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


class MyStyle : public Bamboo::Style {
public:
	void writeHeading(Bamboo::HtmlStreamWriter* writer, const QString& text) const;
	void setHeading(const QString& heading);
	QString content() const;
private:
	QString m_headingStyle;
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


class MyFragment : public Bamboo::Fragment {
public:
	void build(Bamboo::HtmlStreamWriter* writer);
public:
	MyStyle* style;
};

void MyFragment::build(Bamboo::HtmlStreamWriter* writer) {
	QXmlStreamWriter* xml = writer->xmlWriter();
	style->writeHeading(writer, "Test");
	xml->writeTextElement("p", "This is a very simple Test-Page");
}

MainWindow::MainWindow() {
	setupUi(this);


	Bamboo::HtmlStreamWriter writer;
	QBuffer buf;
	buf.open(QIODevice::WriteOnly);
	
	writer.setDevice(&buf);


	MyStyle myStyle;
	myStyle.setHeading("background-color: red");

	
	MyFragment fragment;
	fragment.style = &myStyle;
	Bamboo::Document doc;
	doc.addGlobalStyle(&myStyle);
	doc.setMainFragment(&fragment);
	doc.build(&writer);


	// Create sample resources
	Orchid::Resource::Resource *t1 = new Orchid::Resource::Resource();
	Orchid::Resource::Resource *t2 = new Orchid::Resource::Resource();
	Orchid::Resource::Resource *t3 = new Orchid::Resource::Resource();
	Orchid::Resource::Resource *t4 = new Orchid::Resource::Resource();
	Orchid::ModelResource *res2 = new Orchid::ModelResource();
	Orchid::ModelResource *res = new Orchid::ModelResource();


	res2->addResource("test1", t1);
	res2->addResource("test2", t2),
	res->addResource("test3", t3);
	res->addResource("dir", res2);
	res->addResource("test4", t4);

	m_root.init(res);

	qDebug() << "located: " << Orchid::Resource::Resource::locateUrl(m_root, QUrl("dir/test1")).name();
	
	treeView->setModel(new Orchid::ResourceModel(res, this));
	
// 	webView->setUrl(QUrl("http://localhost:8000/test.html?1"));
	webView->setContent(buf.data());
	sourceView->setPlainText(buf.data());
}
