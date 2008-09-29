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

#include "i18ndocsample.moc"

#include <stem/request.h>

#include <flower/fragment.h>
#include <flower/documentstreams.h>
#include <flower/style.h>
#include <flower/xhtmlstreamwriter.h>
#include <flower/xmlfragmentreader.h>

#include <QtXml/QXmlStreamWriter>
#include <QtCore/QCoreApplication>
#include <QtCore/QTime>
#include <QtCore/QTranslator>
#include <QtCore/QtDebug>

using namespace Orchid;

class I18nDocSampleFragment : public Fragment {
public:
	void build(Orchid::DocumentProcessor* processor);
public:
	QString language;
};

#define TR(x) (useTranslator ? translator.translate("I18n-Sample", x) : x)

void I18nDocSampleFragment::build(DocumentProcessor* processor) {
	using namespace Orchid::Document;
	
	bool useTranslator = true;
	QTranslator translator;
	if(!translator.load("Examples/I18N/i18n_"+language)) {
		qWarning() << "Couldn't find tranlation for language" << language;
		useTranslator = false;
	}

	BlockStream blocks(processor);

	blocks << heading(TR("Internationalization Example"))
	       << paragraph(TR("This example resource demonstrates how to internationalice resources."));

	blocks.paragraph() << markup(TR("Use the <em>Configuration</em> to change the language. (Only \"en\" and \"de\" are supported at moment.)"));

	blocks.paragraph() << markup(TR("By using the <var>XmlFragmentReader</var> you can translate markup code and insert the result into the processor. This works perfectly even if you output the result to different backend."));

	blocks << section
	       << heading(TR("Demonstration"));

	blocks.paragraph() << markup(TR("The current time is <strong>%1</strong>.").arg(QTime::currentTime().toString()));
	
	blocks << end;
};


I18nDocSample::I18nDocSample() {
	m_body = new I18nDocSampleFragment();
	m_body->language = "en";
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

QList<Orchid::Resource::IConfigurable::Option> I18nDocSample::optionList() const {
	QList<Option> list;
	list << Option("language", qMetaTypeId<QString>());
	return list;
}

QVariant I18nDocSample::option(const QString &option) const {
	if(option == "language") {
		return m_body->language;
	}
	return QVariant();
}

bool I18nDocSample::setOption(const QString &option, const QVariant &val) {
	if(option == "language") {
		m_body->language = val.toString();
		return true;
	}
	return false;
}
