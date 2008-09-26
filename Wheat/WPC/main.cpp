/*
 * This file is part of the Orchid tools applications
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtXml/QXmlStreamReader>

#include <flower/xhtmlstreamwriter.h>

#include "cppfragmentwriter.h"
#include "xmlfragmentreader.h"
#include "xmlfragmentwriter.h"
#include "htmlfragmentwriter.h"
#include "fragmentbuilder.h"

#include <QtCore/QStringList>

#include <globals.h>

int process(const QString outFile, const QString &inFile) {
	QFile input(inFile);
	input.open(QIODevice::ReadOnly);

	Orchid::DomFragment* fragment;
	
	QXmlStreamReader xmlIn(&input);
	Orchid::FragmentBuilder builder;
	Orchid::XmlFragmentReader reader(&builder, &xmlIn);
	reader.readDocument();
	fragment = builder.fragment();
	
	if(!fragment) {
		qWarning() << "could not read dom";
		qDebug() << reader.errorLine() << reader.errorColumn() << reader.errorCode() << reader.errorString();
		return 4;
	}
	
	QFile out;
	if(outFile.isEmpty()) {
		out.open(stdout, QIODevice::WriteOnly);
	} else {
		out.setFileName(outFile);
		out.open(QIODevice::WriteOnly);
	}
	
	QTextStream s(&out);
	Orchid::CppHeaderWriter cppWriter(&s);
	cppWriter.addFragment(fragment);
	cppWriter.write();
	s << endl << endl;
	s.flush();

// 	Orchid::XmlFragmentWriter xmlWriter;
// 	QXmlStreamWriter xml(&out);
// 	xml.setAutoFormatting(true);
// 	xml.writeStartDocument();
// 	xmlWriter.write(&xml, fragment);
// 	xml.writeEndDocument();

// 	Orchid::XHtml11StreamWriter html(&out);
// 	html.xmlWriter()->setAutoFormatting(true);
// 	Orchid::HtmlFragmentWriter htmlWriter(&html);
// 	html.startDocument();
// 	htmlWriter.write(fragment);
// 	html.endDocument();

	out.close();
	return 0;
}

void printUsage(const QString &appname) {
	QFile cout;
	cout.open(stdout, QIODevice::WriteOnly);
	QTextStream usage(&cout);
	usage << "Usage: " << appname << " [-o outfile] infile\n";
	cout.close();
}

int main(int argc, char **argv) {
	QCoreApplication app(argc, argv);

	int result = 0;
	QString outFile;
	QString inFile;
	QStringList args = app.arguments();
	QString appname = args[0];
	for(QStringList::iterator it = ++args.begin(); it != args.end(); ++it) {
		if(*it == "-o" || *it == "--output") {
			if(++it == args.end()) {
				printUsage(appname);
				result = 1;
				break;
			}
			outFile = *it;
		} else {
			if(!inFile.isEmpty()) {
				printUsage(appname);
				result = 2;
				break;
			} else {
				inFile = *it;
			}
		}
	}
	
	if(inFile.isEmpty()) {
		printUsage(appname);
		result = 3;
	}
	
	if(!result) {
		result = process(outFile, inFile);
	}

	return result;
}
