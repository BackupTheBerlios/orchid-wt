#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtXml/QXmlStreamReader>

#include "cppfragmentwriter.h"
#include "xmlfragmentreader.h"
#include "xmlfragmentwriter.h"

#include <stdio.h>
#include <QtCore/QDateTime>

#include <globals.h>

void process(const QString& name) {
	QFile input(name);
	input.open(QIODevice::ReadOnly);

	Orchid::DomFragment* fragment;
	
// 	QXmlStreamReader reader(&file);

	qDebug() << "process" << name;
	
	Orchid::XmlFragmentReader reader;
	QXmlStreamReader xmlIn(&input);
	fragment = reader.read(&xmlIn);
	
	if(!fragment) {
		qWarning() << "could not read dom";
		qDebug() << reader.errorCode() << reader.errorString();
		return;
	}
	
	qDebug() << "output dom";
	

	QFile cout;
	cout.open(stdout, QIODevice::WriteOnly);
	
// 	Orchid::CppFragmentWriter writer(&cout);
	QTextStream s(&cout);
	Orchid::CppHeaderWriter writer(&s);
	writer.addFragment(fragment);
// 	writer.writeCopyrightHeader(&s);
// 	writer.writeHeaderProtectionStart(&s);
// 	writer.writeHeaderProtectionEnd(&s);
	writer.write();

// 	Orchid::XmlFragmentWriter writer;
// 	QXmlStreamWriter xml(&cout);
// 	writer.write(&xml, fragment);
// 	xml.writeEndDocument();

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
