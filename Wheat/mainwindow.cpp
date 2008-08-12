#include "mainwindow.h"

#include "mainwindow.moc"

#include <QBuffer>
#include <QtDebug>
#include <QXmlStreamWriter>

#include <flower/htmlstreamwriter.h>

#include <flower/document.h>

#include <flower/style.h>
#include <flower/styleattributes.h>

#include <stem/resourcemodel.h>
#include <stem/request.h>

#include <flower/htmlstreams.h>


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
	using namespace Bamboo::HTML;

	BlockStream blocks(writer);

	blocks << heading("Top") << section << heading("Test");
	blocks.text() << "some inline text without paragraph or that like";

	blocks.paragraph() << "The Pascal statement " <<code<<"i := 1;"<<end<< " assigns the literal value one to the variable <var>i</var>.";

	(blocks << role(Bamboo::HtmlRoleDefinition)).paragraph() << "An " <<id("def-acronym")<< definition<<"acronym"<<end<< " is a word formed from the initial letters or groups of letters of words in a set phrase or series of words.";

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

void BambooResource::addStyle(Bamboo::Style* style) {
	m_doc.addGlobalStyle(style);
}

void BambooResource::setMainFragment(Bamboo::Fragment* fragment) {
	m_doc.setMainFragment(fragment);
}

void BambooResource::query(Orchid::Request* request) {
	if(!(request->method() & Orchid::GetMethod)) {
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

	res->addResource("sample.html", sample);
	dir->addResource("text.txt", text);
	res->addResource("dir", dir);
	res->addResource("test1", t1);
	res->addResource("test2", t2),

	m_root.init(res);
	
	m_service.setRoot(m_root);

	m_model = new Orchid::ResourceModel(res, this);
	treeView->setModel(m_model);
	connect(treeView, SIGNAL(activated(const QModelIndex&)), this, SLOT(activateResource(const QModelIndex&)));

	webView->setUrl(QUrl("http://localhost:8000/sample.html?1"));
	
	// TODO load sourceView from local server
// 	sourceView->setPlainText(buf.data());
}

void MainWindow::activateResource(const QModelIndex& index) {
	Orchid::Resource::Handle handle(m_model->resource(index));
	Orchid::Resource::IQueryable* res = dynamic_cast<Orchid::Resource::IQueryable*>(handle.resource());

	if(res) {
		QBuffer buf;
		Orchid::SimpleRequest request;
		request.setDevice(&buf);
		res->query(&request);
		sourceView->setPlainText(buf.data());
	}

	QString url = "http://localhost:8000" + m_model->path(index);
	webView->setUrl(QUrl(url));
}
