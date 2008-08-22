#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtXml/QXmlStreamReader>

#include <flower/htmlstreamwriter.h>

#include "cppfragmentwriter.h"
#include "xmlfragmentreader.h"
#include "xmlfragmentwriter.h"
#include "htmlfragmentwriter.h"

#include <stdio.h>
#include <QtCore/QDateTime>

#include <globals.h>

void process(const QString& name) {
	QFile input(name);
	input.open(QIODevice::ReadOnly);

	Orchid::DomFragment* fragment;
	
	qDebug() << "process" << name;
	
	QXmlStreamReader xmlIn(&input);
	Orchid::XmlFragmentReader reader(&xmlIn);
	fragment = reader.read();
	
	if(!fragment) {
		qWarning() << "could not read dom";
		qDebug() << reader.errorLine() << reader.errorColumn() << reader.errorCode() << reader.errorString();
		return;
	}
	
	qDebug() << "output dom";
	

	QFile cout;
	cout.open(stdout, QIODevice::WriteOnly);
	
	QTextStream s(&cout);
	Orchid::CppHeaderWriter cppWriter(&s);
	cppWriter.addFragment(fragment);
	cppWriter.write();
	s << endl << endl;
	s.flush();

	Orchid::XmlFragmentWriter xmlWriter;
	QXmlStreamWriter xml(&cout);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xmlWriter.write(&xml, fragment);
	xml.writeEndDocument();

	Orchid::XHtml11StreamWriter html(&cout);
	html.xmlWriter()->setAutoFormatting(true);
	Orchid::HtmlFragmentWriter htmlWriter(&html);
	htmlWriter.write(fragment);
	html.xmlWriter()->writeEndDocument();

	cout.close();
}


int main(int argc, char **argv) {
	QCoreApplication app(argc, argv);

	// skip programm name
	++argv;
	for(; argc != 0; --argc) {
		QString name(*(argv++));
		if(name.isEmpty()) continue;
		process(name);
	}

	return 0;
}
