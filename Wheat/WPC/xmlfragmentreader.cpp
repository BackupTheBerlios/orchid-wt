#include "xmlfragmentreader.h"

#include <QtCore/QHash>
#include <QtXml/QXmlStreamReader>
#include "fragmentdom.h"

#include <QtCore/QtDebug> // TODO rm this

namespace Orchid {

class XmlFragmentReaderHelper {
private:
	XmlFragmentReaderHelper();
public:
	static XmlFragmentReaderHelper* inst();
public:
	HtmlTag tag(const QStringRef& string) const;
private:
	void setup();
private:
	QString m_tagSection;
	QString m_tagHeading;
	QString m_tagParagraph;
	QString m_tagTextAbbreviation;
	QString m_tagTextCode;	
	QString m_tagTextDefinition;
	QString m_tagTextEmphasis;
	QString m_tagTextKeyboard;
	QString m_tagTextQuote;
	QString m_tagTextSample;
	QString m_tagTextSpan;
	QString m_tagTextStrong;
	QString m_tagTextSubscript;
	QString m_tagTextSuperscript;
	QString m_tagTextVariable;
	QString m_tagText;
	QHash<QStringRef, HtmlTag> m_tagLookup;
};

XmlFragmentReaderHelper* XmlFragmentReaderHelper::inst() {
	static XmlFragmentReaderHelper helper;
	return &helper;
}

XmlFragmentReaderHelper::XmlFragmentReaderHelper() {
	m_tagSection = "section";
	m_tagLookup.insert(&m_tagSection, HtmlTagSection);
	m_tagHeading = "h";
	m_tagLookup.insert(&m_tagHeading, HtmlTagHeading);
	m_tagParagraph = "p";
	m_tagLookup.insert(&m_tagParagraph, HtmlTagParagraph);
	m_tagTextAbbreviation = "abbr";
	m_tagLookup.insert(&m_tagTextAbbreviation, HtmlTagTextAbbreviation);
	m_tagTextCode = "code";
	m_tagLookup.insert(&m_tagTextCode, HtmlTagTextCode);
	m_tagTextDefinition = "dfn";
	m_tagLookup.insert(&m_tagTextDefinition, HtmlTagTextDefinition);
	m_tagTextEmphasis = "em";
	m_tagLookup.insert(&m_tagTextEmphasis, HtmlTagTextEmphasis);
	m_tagTextKeyboard = "kbd";
	m_tagLookup.insert(&m_tagTextKeyboard, HtmlTagTextKeyboard);
	m_tagTextQuote = "q";
	m_tagLookup.insert(&m_tagTextQuote, HtmlTagTextQuote);
	m_tagTextSample = "samp";
	m_tagLookup.insert(&m_tagTextSample, HtmlTagTextSample);
	m_tagTextStrong = "strong";
	m_tagLookup.insert(&m_tagTextStrong, HtmlTagTextStrong);
	m_tagTextSubscript = "sub";
	m_tagLookup.insert(&m_tagTextSubscript, HtmlTagTextSubscript);
	m_tagTextSuperscript = "sup";
	m_tagLookup.insert(&m_tagTextSuperscript, HtmlTagTextSuperscript);
	m_tagTextVariable = "var";
	m_tagLookup.insert(&m_tagTextVariable, HtmlTagTextVariable);
}

HtmlTag XmlFragmentReaderHelper::tag(const QStringRef& name) const {
	return m_tagLookup.value(name);
}


class XmlFragmentReaderPrivate {
public:
	XmlFragmentReaderPrivate(XmlFragmentReader *reader)
	 : q_ptr (reader), errorCode(XmlFragmentReader::NoError)
	{ }
public:
	void readHeading(DomHeading* heading, QXmlStreamReader* xml);
	void readSection(DomSection* section, QXmlStreamReader* xml);
	void readFragment(DomFragment* fragment, QXmlStreamReader* xml);
	void error(XmlFragmentReader::ErrorCode code, const QString& text);
private:
	XmlFragmentReader::ErrorCode errorCode;
	QString errorString;
private:
	XmlFragmentReader* q_ptr;
	Q_DECLARE_PUBLIC(XmlFragmentReader);
};

void XmlFragmentReaderPrivate::error(XmlFragmentReader::ErrorCode code, const QString& text) {
	errorCode = code;
	errorString = text;
}

void XmlFragmentReaderPrivate::readHeading(DomHeading* heading, QXmlStreamReader* xml) {
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			DomCharacters* chars = new DomCharacters();
			chars->setText(xml->text().toString());
			heading->append(chars);
		} else if(xml->isStartElement()) {
			{
				error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
			}
		} else if(xml->isEndElement()) {
			return;
		}
	}
}

void XmlFragmentReaderPrivate::readSection(DomSection* section, QXmlStreamReader* xml) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			switch(helper->tag(xml->name())) {
				case HtmlTagSection: {
					DomSection* sub = new DomSection();
					readSection(sub, xml);
					section->append(sub);
				} break;
				case HtmlTagHeading: {
					DomHeading* heading = new DomHeading();
					readHeading(heading, xml);
					section->append(heading);
				} break;
				default: {
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
				}
			}
		} else if(xml->isEndElement()) {
			return;
		}
	}
}


void XmlFragmentReaderPrivate::readFragment(DomFragment* fragment, QXmlStreamReader* xml) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			switch(helper->tag(xml->name())) {
				case HtmlTagSection: {
					DomSection* sub = new DomSection();
					readSection(sub, xml);
					fragment->append(sub);
				} break;
				case HtmlTagHeading: {
					DomHeading* heading = new DomHeading();
					readHeading(heading, xml);
					fragment->append(heading);
				} break;
				default: {
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
				}
			}
		} else if(xml->isEndElement()) {
			return;
		}
	}
}



XmlFragmentReader::XmlFragmentReader() {
	d_ptr = new XmlFragmentReaderPrivate(this);
}

XmlFragmentReader::~XmlFragmentReader() {
	delete d_ptr;
}

DomFragment* XmlFragmentReader::read(QXmlStreamReader* xml) {
	Q_D(XmlFragmentReader);
	DomFragment* fragment = 0;
	
	while(!xml->atEnd() && !d->errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			if(xml->name() == "fragment") {
				fragment = new DomFragment();
				d->readFragment(fragment, xml);
			} else {
				d->error(UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
			}
		}
	}
	if(d->errorCode) {
		delete fragment;
		fragment = 0;
	}
	return fragment;
}

XmlFragmentReader::ErrorCode XmlFragmentReader::errorCode() const {
	Q_D(const XmlFragmentReader);
	return d->errorCode;
}

QString XmlFragmentReader::errorString() const {
	Q_D(const XmlFragmentReader);
	return d->errorString;
}

}
