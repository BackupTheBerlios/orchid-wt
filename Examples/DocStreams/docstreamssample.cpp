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


#include "docstreamssample.h"

#include <stem/request.h>

#include <flower/fragment.h>
#include <flower/documentstreams.h>
#include <flower/xhtmlstreamwriter.h>
#include <flower/style.h>

#include <QtXml/QXmlStreamWriter>

using namespace Orchid;

class DocStreamsSampleFragment : public Fragment {
public:
	void build(Orchid::DocumentProcessor* writer);
public:
	Style* style;
};

void DocStreamsSampleFragment::build(DocumentProcessor* writer) {
	using namespace Orchid::Document;

	BlockStream blocks(writer);

	blocks << heading("Top") << section << heading("Sub");

	blocks.paragraph() << "The Pascal statement " <<code<<"i := 1;"<<end<< " assigns the literal value one to the variable "<<variable<<'i'<<end<<'.';

	(blocks << role(Orchid::Document::RoleDefinition)).paragraph() << "An " <<id("def-acronym")<< definition<<"acronym"<<end<< " is a word formed from the initial letters or groups of letters of words in a set phrase or series of words.";

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
};


class DocStreamsSampleStyle : public Orchid::Style {
public:
	void setHeading(const QString& heading);
	QString content() const;
private:
	QString m_headingStyle;
};

void DocStreamsSampleStyle::setHeading(const QString& heading) {
	m_headingStyle = heading;
}

QString DocStreamsSampleStyle::content() const {
	return QString(".heading1 { ") + m_headingStyle + "}";
}


DocStreamsSample::DocStreamsSample() {
	DocStreamsSampleStyle *style = new 	DocStreamsSampleStyle();
	DocStreamsSampleFragment *body = new DocStreamsSampleFragment();

	style->setHeading("background-color: red");
	body->style = style;
	m_head.addStyle(style);

	m_style = style;
	m_body = body;
}

DocStreamsSample::~DocStreamsSample() {
	delete m_body;
	delete m_style;
}

void DocStreamsSample::query(Orchid::Request* request) {
	if(!(request->method() & Orchid::GetMethod)) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;
	
	Orchid::XHtml11StreamWriter writer;
	writer.setDevice(request);
	writer.xmlWriter()->setAutoFormatting(true);
	m_head.setTitle("Sample");
	writer.startDocument(m_head);
	m_body->build(&writer);
	writer.endDocument();
}
