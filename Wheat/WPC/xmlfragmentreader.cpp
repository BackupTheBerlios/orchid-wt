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
	m_tagTextSpan = "span";
	m_tagLookup.insert(&m_tagTextSpan, HtmlTagTextSpan);
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
	DomTextElement* readInline(HtmlTag tag);
	DomParagraph* readParagraph();
	DomHeading* readHeading();
	DomSection* readSection();
	DomFragment* readFragment();
	void error(XmlFragmentReader::ErrorCode code, const QString& text);
private:
	QXmlStreamReader* xml;
	XmlFragmentReader::ErrorCode errorCode;
	QString errorString;
	int errorLine;
	int errorColumn;
private:
	XmlFragmentReader* q_ptr;
	Q_DECLARE_PUBLIC(XmlFragmentReader);
};

void XmlFragmentReaderPrivate::error(XmlFragmentReader::ErrorCode code, const QString& text) {
	errorCode = code;
	errorString = text;
	errorLine = xml->lineNumber();
	errorColumn = xml->columnNumber();
}

DomTextElement* XmlFragmentReaderPrivate::readInline(HtmlTag tag) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	DomTextElement* element = new DomTextElement(tag);
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			DomCharacters* chars = new DomCharacters();
			chars->setText(xml->text().toString());
			element->append(chars);
		} else if(xml->isStartElement()) {
			HtmlTag tag = helper->tag(xml->name());
			switch(tag) {
				case HtmlTagTextAbbreviation:
				case HtmlTagTextCode:
				case HtmlTagTextDefinition:
				case HtmlTagTextEmphasis:
				case HtmlTagTextKeyboard:
				case HtmlTagTextQuote:
				case HtmlTagTextSample:
				case HtmlTagTextSpan:
				case HtmlTagTextStrong:
				case HtmlTagTextSubscript:
				case HtmlTagTextSuperscript:
				case HtmlTagTextVariable:
					element->append(readInline(tag));
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else  if(xml->isEndElement()) {
			return element;
		}
	}
	delete element;
	return 0;
}

DomHeading* XmlFragmentReaderPrivate::readHeading() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	DomHeading* heading = new DomHeading();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			DomCharacters* chars = new DomCharacters();
			chars->setText(xml->text().toString());
			heading->append(chars);
		} else if(xml->isStartElement()) {
			HtmlTag tag = helper->tag(xml->name());
			switch(tag) {
				case HtmlTagTextAbbreviation:
				case HtmlTagTextCode:
				case HtmlTagTextDefinition:
				case HtmlTagTextEmphasis:
				case HtmlTagTextKeyboard:
				case HtmlTagTextQuote:
				case HtmlTagTextSample:
				case HtmlTagTextSpan:
				case HtmlTagTextStrong:
				case HtmlTagTextSubscript:
				case HtmlTagTextSuperscript:
				case HtmlTagTextVariable:
					heading->append(readInline(tag));
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else if(xml->isEndElement()) {
			return heading;
		}
	}
	delete heading;
	return 0;
}

DomParagraph* XmlFragmentReaderPrivate::readParagraph() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	DomParagraph* paragraph = new DomParagraph();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			DomCharacters* chars = new DomCharacters();
			chars->setText(xml->text().toString());
			paragraph->append(chars);
		} else if(xml->isStartElement()) {
			HtmlTag tag = helper->tag(xml->name());
			switch(tag) {
				case HtmlTagTextAbbreviation:
				case HtmlTagTextCode:
				case HtmlTagTextDefinition:
				case HtmlTagTextEmphasis:
				case HtmlTagTextKeyboard:
				case HtmlTagTextQuote:
				case HtmlTagTextSample:
				case HtmlTagTextSpan:
				case HtmlTagTextStrong:
				case HtmlTagTextSubscript:
				case HtmlTagTextSuperscript:
				case HtmlTagTextVariable:
					paragraph->append(readInline(tag));
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else  if(xml->isEndElement()) {
			return paragraph;
		}
	}
	delete paragraph;
	return 0;
}

DomSection*  XmlFragmentReaderPrivate::readSection() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	DomSection* section = new DomSection();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			switch(helper->tag(xml->name())) {
				case HtmlTagSection: section->append(readSection()); break;
				case HtmlTagHeading: section->append(readHeading()); break;
				case HtmlTagParagraph: section->append(readParagraph()); break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else if(xml->isEndElement()) {
			return section;
		}
	}
	delete section;
	return 0;
}


DomFragment* XmlFragmentReaderPrivate::readFragment() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	DomFragment* fragment = new DomFragment();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			switch(helper->tag(xml->name())) {
				case HtmlTagSection: fragment->append(readSection()); break;
				case HtmlTagHeading: fragment->append(readHeading()); break;
				case HtmlTagParagraph: fragment->append(readParagraph()); break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else if(xml->isEndElement()) {
			return fragment;
		}
	}
	delete fragment;
	return 0;
}



XmlFragmentReader::XmlFragmentReader(QXmlStreamReader* xml) {
	d_ptr = new XmlFragmentReaderPrivate(this);
	Q_D(XmlFragmentReader);
	d->xml = xml;
}

XmlFragmentReader::~XmlFragmentReader() {
	delete d_ptr;
}

DomFragment* XmlFragmentReader::read() {
	Q_D(XmlFragmentReader);
	DomFragment* fragment = 0;
	
	while(!d->xml->atEnd() && !d->errorCode) {
		d->xml->readNext();
		if(d->xml->isStartElement()) {
			if(d->xml->name() == "fragment") {
				fragment = d->readFragment();
			} else {
				d->error(UnallowedElement, QString("'%1' not allowed here").arg(d->xml->name().toString()));
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

int XmlFragmentReader::errorLine() const {
	Q_D(const XmlFragmentReader);
	return d->errorLine;
}

int XmlFragmentReader::errorColumn() const {
	Q_D(const XmlFragmentReader);
	return d->errorColumn;
}

}
