#include "mainwindow.h"

#include "mainwindow.moc"

#include <QBuffer>
#include <QtDebug>
#include <QXmlStreamWriter>

#include <flower/htmlstreamwriter.h>

#include <flower/style.h>
#include <flower/styleattributes.h>

#include <stem/request.h>
#include <stem/resourcemodel.h>
#include <stem/location.h>

#include <flower/htmlstreams.h>

#include <leaf/xmlmodelresource.h>
#include <leaf/imageresource.h>
#include <leaf/scriptedresource.h>

#include <QFile>

#include "gallery.h"



class OrchidResource : public Orchid::Resource::IResource, public Orchid::Resource::IQueryable {
public:
	void addStyle(Orchid::Style* style);
	void setMainFragment(Orchid::Fragment* fragment);
	void query(Orchid::Request* request);
private:
	Orchid::HtmlHead m_head;
	Orchid::Fragment* m_body;
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
	m_head.addStyle(style);
}

void OrchidResource::setMainFragment(Orchid::Fragment* fragment) {
	m_body = fragment;
}

void OrchidResource::query(Orchid::Request* request) {
	if(!(request->method() & Orchid::GetMethod)) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;
	
	Orchid::XHtml11StreamWriter writer;
	writer.setDevice(request);
	writer.xmlWriter()->setAutoFormatting(true);
	m_head.setTitle("Sample.html");
	writer.writeStartDocument(m_head);
	m_body->build(&writer);
	writer.writeEndDocument();
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
	
	Gallery* gal = new Gallery();
	gal->insertFile("image.jpg", "test.jpg");
	gal->insertFile("test.jpg", "test.jpg");
	res->addResource("gallery", gal);
	
	QFile scriptFile(":/test.js");
	scriptFile.open(QIODevice::ReadOnly);
	QString program(scriptFile.readAll());
	scriptFile.close();
	
	res->addResource("scripted", new Orchid::ScriptedResource(program, "MyResource"));

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
