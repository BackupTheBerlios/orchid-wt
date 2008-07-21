#include "mainwindow.h"

#include "mainwindow.moc"

#include <QBuffer>
#include <QtDebug>
#include <QXmlStreamWriter>

#include "builder.h"

#include "document.h"

#include "style.h"
#include "styleattributes.h"

class MyStyle : public Bamboo::Style {
public:
	void writeHeading(Bamboo::Builder* builder, const QString& text) const;
	void setHeading(const QString& heading);
	QString content() const;
private:
	QString m_headingStyle;
};

void MyStyle::writeHeading(Bamboo::Builder* builder, const QString& text) const {
	Bamboo::StyleAttributes attrs = builder->attributes(this);
	QXmlStreamWriter* xml = builder->xml();

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
	void build(Bamboo::Builder* builder);
public:
	MyStyle* style;
};

void MyFragment::build(Bamboo::Builder* builder) {
	QXmlStreamWriter* writer = builder->xml();
	style->writeHeading(builder, "Test");
	writer->writeTextElement("p", "This is a very simple Test-Page");
}

MainWindow::MainWindow() {
	setupUi(this);
	Bamboo::Builder builder;
	QBuffer buf;
	buf.open(QIODevice::WriteOnly);
	
	builder.setDevice(&buf);

	MyStyle myStyle;
	myStyle.setHeading("background-color: red");
	
	MyFragment fragment;
	fragment.style = &myStyle;
	Bamboo::Document doc;
	doc.addGlobalStyle(&myStyle);
	doc.setMainFragment(&fragment);
	doc.build(&builder);
	
	webView->setContent(buf.data());
	sourceView->setPlainText(buf.data());
}
