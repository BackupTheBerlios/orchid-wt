/*
 * This file is part of the Orchid example plugins
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "i18ndocsample.h"

#include <stem/request.h>

#include <flower/fragment.h>
#include <flower/documentstreams.h>
#include <flower/style.h>
#include <flower/xhtmlstreamwriter.h>
#include <flower/xmlfragmentreader.h>

#include <QtXml/QXmlStreamWriter>
#include <QtCore/QCoreApplication>

using namespace Orchid;

class I18nDocSampleFragment : public Fragment {
public:
	void build(Orchid::DocumentProcessor* processor);
};

void I18nDocSampleFragment::build(DocumentProcessor* processor) {
	using namespace Orchid::Document;

	XmlFragmentReader reader(processor);

	BlockStream blocks(processor);

	blocks << heading(QCoreApplication::translate("Test", "Top"))
	       << section << heading(QCoreApplication::translate("Test", "Sub"));

	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "The Pascal statement <code>i := 1;</code> assigns the literal value one to the variable <var>i</var>."));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "An <dfn>acronym</dfn> is a word formed from the initial letters or groups of letters of words in a set phrase or series of words."));
	reader.readInline();
	processor->endElement();

	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "Do <em>not</em> phone before 9 a.m."));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "To exit, type <kbd>QUIT</kbd>."));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "I'm currently at Akademy an event of the <abbr>KDE.</abbr>"));
	reader.readInline();
	processor->endElement();

// 	blocks.blockcode() << styleclass("program") << line << "program p(input, output);"<<end<<line<<"begin"<<end<<line<<"   writeln(\"Hello world\");"<<end<<line<<"end."<<end;

	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "John said, <q>\"I saw Lucy at lunch, she told me <q>'Mary wants you to get some ice cream on your way home.'</q> I think I will get some at Jen and Berry's, on Gloucester Road.\"</q>"));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "On starting, you will see the prompt <samp>$ </samp>."));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "This operation is called the <span>transpose</span> or <span>inverse</span>."));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "On <strong>Monday</strong> please put the rubbish out, but <em>not</em> before nightfall!"));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "H<sub>2</sub>O"));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "E = mc<sup>2</sup>"));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "<span>M<sup>lle</sup> Dupont</span>"));
	reader.readInline();
	processor->endElement();
	
	processor->startElement(TagParagraph);
	reader.setInlineString(QCoreApplication::translate("Test", "The parameter <var>ncols</var> represents the number of colors to use."));
	reader.readInline();
	processor->endElement();

	blocks << end;
};


I18nDocSample::I18nDocSample() {
	m_body = new I18nDocSampleFragment();
}

I18nDocSample::~I18nDocSample() {
	delete m_body;
}

void I18nDocSample::query(Orchid::Request* request) {
	if(!(request->method() & Orchid::GetMethod)) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;
	
	Orchid::XHtml11StreamWriter writer;
	writer.setDevice(request);
	writer.xmlWriter()->setAutoFormatting(true);
	m_head.setTitle("I18n-Document-Sample");
	writer.startDocument(m_head);
	m_body->build(&writer);
	writer.endDocument();
}
