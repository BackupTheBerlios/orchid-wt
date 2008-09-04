#include "mainwindow.h"

#include "mainwindow.moc"

#include <QBuffer>
#include <QtDebug>
#include <QXmlStreamWriter>

#include <flower/htmlstreamwriter.h>

#include <flower/document.h>

#include <flower/style.h>
#include <flower/styleattributes.h>

#include <stem/request.h>
#include <stem/resourcemodel.h>
#include <stem/location.h>

#include <flower/htmlstreams.h>

#include <leaf/xmlmodelresource.h>
#include <leaf/imageresource.h>

class Gallery : public Orchid::Resource::IResource, public Orchid::Resource::IQueryable, public Orchid::Resource::IDirectory {
public:
	Gallery(const QString& file);
public:
	void query(Orchid::Request*);
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString&);
private:
	QString m_file;
	Orchid::Resource::Keep m_keep;
};

Gallery::Gallery(const QString& file) {
	m_file = file;
}

void Gallery::query(Orchid::Request *request) {
	// TODO generate simple gallery html
	if(!(request->method() & Orchid::GetMethod)) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;
	
	Orchid::XHtml11StreamWriter writer;
	writer.setDevice(request);
	writer.xmlWriter()->setAutoFormatting(true);
	
	writer.writeStartDocument();
	writer.writeStartElement(Orchid::HtmlTagHeading);
	writer.writeCharacters("Gallery");
	writer.writeEndElement();
	writer.xmlWriter()->writeEmptyElement("img");
	writer.xmlWriter()->writeAttribute("src", request->url(request->location().relative(m_file)));
	writer.writeEndDocument();
}

QStringList Gallery::childs() const {
	return QStringList() << m_file;
}

Orchid::Resource::Handle Gallery::child(const QString &name) {
	if(name != m_file) return Orchid::Resource::Handle();
	Orchid::Resource::Handle handle = m_keep.getHandle(name);
	if(handle.isEmpty()) {
		handle.init(new Orchid::ImageResource(m_file));
	}
	return handle;
}



class OrchidResource : public Orchid::Resource::IResource, public Orchid::Resource::IQueryable {
public:
	void addStyle(Orchid::Style* style);
	void setMainFragment(Orchid::Fragment* fragment);
	void query(Orchid::Request* request);
private:
	Orchid::Document m_doc;
};


void MyStyle::writeHeading(Orchid::HtmlStreamWriter* writer, const QString& text) const {
	Orchid::StyleAttributes attrs = writer->attributes(this);
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


void MyFragment::build(Orchid::HtmlStreamWriter* writer) {
	using namespace Orchid::HTML;

	BlockStream blocks(writer);

	blocks << heading("Top") << section << heading("Sub");

	blocks.paragraph() << "The Pascal statement " <<code<<"i := 1;"<<end<< " assigns the literal value one to the variable "<<variable<<'i'<<end<<'.';

	(blocks << role(Orchid::HtmlRoleDefinition)).paragraph() << "An " <<id("def-acronym")<< definition<<"acronym"<<end<< " is a word formed from the initial letters or groups of letters of words in a set phrase or series of words.";

	blocks.paragraph() << "Do " <<emphasis<<"not"<<end<< " phone before 9 a.m.";

	blocks.paragraph() << "To exit, type " <<keyboard<<"QUIT"<<end <<'.';

	blocks.paragraph() << "I'm currently at Akademy an event of the " << abbreviation("K Desktop Environment") << "KDE." << end;

// 	blocks.blockcode() << styleclass("program") << line << "program p(input, output);"<<end<<line<<"begin"<<end<<line<<"   writeln(\"Hello world\");"<<end<<line<<"end."<<end;

	blocks.paragraph() << "John said, " <<quote/*("\"")*/<<"\"I saw Lucy at lunch, she told me "<< quote/*("'")*/<<"\'Mary wants you to get some ice cream on your way home.\'"<<end<< " I think I will get some at Jen and Berry's, on Gloucester Road.\""<<end;

	blocks.paragraph() << "On starting, you will see the prompt " <<sample<<"$ "<<end<<'.';

	blocks.paragraph() << "This operation is called the "<<classname("xref")<<span <<"transpose"<<end<<" or "<<classname("xref")<<span<<"inverse"<<end<<'.';

	blocks.paragraph() << "On "<<strong<<"Monday"<<end<<" please put the rubbish out, but "<<emphasis<<"not"<<end<<" before nightfall!";

	blocks.paragraph() << 'H'<<subscript<<'2'<<end<<'O';

    blocks.paragraph() << "E = mc"<<superscript<<'2'<<end;
	blocks.paragraph() << language("fr") << span<<'M'<<superscript<<"lle"<<end<<" Dupont"<<end;

	blocks.paragraph() << "The parameter "<<variable<<"ncols"<<end<<" represents the number of colors to use.";

// 	Html(tr("I'm currently at Akademy an event of the %1")).arg(abbreviation(tr("K Desktop Environment")));
	blocks << end;
}

void OrchidResource::addStyle(Orchid::Style* style) {
	m_doc.addGlobalStyle(style);
}

void OrchidResource::setMainFragment(Orchid::Fragment* fragment) {
	m_doc.setMainFragment(fragment);
}

void OrchidResource::query(Orchid::Request* request) {
	if(!(request->method() & Orchid::GetMethod)) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;
	
	Orchid::XHtml11StreamWriter writer;
	writer.setDevice(request);
	m_doc.build(&writer);
	
}

MainWindow::MainWindow() : m_service(8000) {
	setupUi(this);

	m_style.setHeading("background-color: red");
	m_fragment.style = &m_style;
	
	OrchidResource *sample = new OrchidResource();
	sample->addStyle(&m_style);
	sample->setMainFragment(&m_fragment);

	Orchid::SimpleTextResource *text = new Orchid::SimpleTextResource("Hello World");

	// Create sample resources
	Orchid::ContainerResource *dir = new Orchid::ContainerResource();
	Orchid::ContainerResource *res = new Orchid::ContainerResource();

	res->addResource("sample.html", sample);
	dir->addResource("text.txt", text);
	res->addResource("dir", dir);

	m_root.init(res);
	
	m_service.setRoot(m_root);

	m_model = new Orchid::ResourceModel(res, this);

	res->addResource("resource.model", new Orchid::XmlModelResource(m_model));

	res->addResource("image.jpg", new Orchid::ImageResource("test.jpg"));
	res->addResource("gallery", new Gallery("test.jpg"));

	treeView->setModel(m_model);
	connect(treeView, SIGNAL(activated(const QModelIndex&)), this, SLOT(activateResource(const QModelIndex&)));

	
	connect(&reader, SIGNAL(requestFinished(int,bool)), this, SLOT(requestFinished(int,bool)));
	reader.setHost("localhost", 8000);
}

void MainWindow::activateResource(const QModelIndex& index) {
	Orchid::Resource::Handle handle(m_model->resource(index));
	Orchid::Resource::IQueryable* res = dynamic_cast<Orchid::Resource::IQueryable*>(handle.resource());

	QString path = m_model->path(index);
	reader.get(path, &result);
}

void MainWindow::requestFinished(int id, bool error) {
	if(!error) {
		result.close();
		QByteArray content = result.data();
		sourceView->setPlainText(content);
		webView->setContent(content, "text/html", QUrl("http://localhost:8000/"));
		result.setData(QByteArray());
	} else {
		sourceView->setPlainText("\n\n\t\tError reading resource!\n\n"+reader.errorString());
		webView->setContent("<h1>Error</h1>Couldn't read resource"+reader.errorString().toUtf8());
	}
}
