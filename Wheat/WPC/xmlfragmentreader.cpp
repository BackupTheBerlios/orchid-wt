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
	void readInlineContent(DomTextElement* element);
	void readParagraph(DomParagraph* paragraph);
	void readHeading(DomHeading* heading);
	void readSection(DomSection* section);
	void readFragment(DomFragment* fragment);
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

void XmlFragmentReaderPrivate::readInlineContent(DomTextElement* element) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
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
				case HtmlTagTextVariable: {
					DomTextElement* child = new DomTextElement(tag);
					readInlineContent(child);
					element->append(child);
				} break;
				default: {
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
				} break;
			}
		} else  if(xml->isEndElement()) {
			return;
		}
	}
}

void XmlFragmentReaderPrivate::readHeading(DomHeading* heading) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
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
				case HtmlTagTextVariable: {
					DomTextElement* child = new DomTextElement(tag);
					readInlineContent(child);
					heading->append(child);
				} break;
				default: {
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
				} break;
			}
		} else if(xml->isEndElement()) {
			return;
		}
	}
}

void XmlFragmentReaderPrivate::readParagraph(DomParagraph* paragraph) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
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
				case HtmlTagTextVariable: {
					DomTextElement* child = new DomTextElement(tag);
					readInlineContent(child);
					paragraph->append(child);
				} break;
				default: {
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
				} break;
			}
		} else  if(xml->isEndElement()) {
			return;
		}
	}
}

void XmlFragmentReaderPrivate::readSection(DomSection* section) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			switch(helper->tag(xml->name())) {
				case HtmlTagSection: {
					DomSection* sub = new DomSection();
					readSection(sub);
					section->append(sub);
				} break;
				case HtmlTagHeading: {
					DomHeading* heading = new DomHeading();
					readHeading(heading);
					section->append(heading);
				} break;
				case HtmlTagParagraph: {
					DomParagraph* paragraph = new DomParagraph();
					readParagraph(paragraph);
					section->append(paragraph);
				} break;
				default: {
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
				} break;
			}
		} else if(xml->isEndElement()) {
			return;
		}
	}
}


void XmlFragmentReaderPrivate::readFragment(DomFragment* fragment) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			switch(helper->tag(xml->name())) {
				case HtmlTagSection: {
					DomSection* sub = new DomSection();
					readSection(sub);
					fragment->append(sub);
				} break;
				case HtmlTagHeading: {
					DomHeading* heading = new DomHeading();
					readHeading(heading);
					fragment->append(heading);
				} break;
				default: {
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
				} break;
			}
		} else if(xml->isEndElement()) {
			return;
		}
	}
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
				fragment = new DomFragment();
				d->readFragment(fragment);
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
