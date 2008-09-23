#include "xmlfragmentreader.h"

#include <QtCore/QHash>
#include <QtXml/QXmlStreamReader>
#include "fragmentdom.h"
#include "fragmentbuilder.h"

#include <QtCore/QtDebug> // TODO rm this

namespace Orchid {

class XmlFragmentReaderHelper {
private:
	XmlFragmentReaderHelper();
public:
	static XmlFragmentReaderHelper* inst();
public:
	Document::Tag tag(const QStringRef& string) const;
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
	QHash<QStringRef, Document::Tag> m_tagLookup;
};

XmlFragmentReaderHelper* XmlFragmentReaderHelper::inst() {
	static XmlFragmentReaderHelper helper;
	return &helper;
}

XmlFragmentReaderHelper::XmlFragmentReaderHelper() {
	m_tagSection = "section";
	m_tagLookup.insert(&m_tagSection, Document::TagSection);
	m_tagHeading = "h";
	m_tagLookup.insert(&m_tagHeading, Document::TagHeading);
	m_tagParagraph = "p";
	m_tagLookup.insert(&m_tagParagraph, Document::TagParagraph);
	m_tagTextAbbreviation = "abbr";
	m_tagLookup.insert(&m_tagTextAbbreviation, Document::TagTextAbbreviation);
	m_tagTextCode = "code";
	m_tagLookup.insert(&m_tagTextCode, Document::TagTextCode);
	m_tagTextDefinition = "dfn";
	m_tagLookup.insert(&m_tagTextDefinition, Document::TagTextDefinition);
	m_tagTextEmphasis = "em";
	m_tagLookup.insert(&m_tagTextEmphasis, Document::TagTextEmphasis);
	m_tagTextKeyboard = "kbd";
	m_tagLookup.insert(&m_tagTextKeyboard, Document::TagTextKeyboard);
	m_tagTextQuote = "q";
	m_tagLookup.insert(&m_tagTextQuote, Document::TagTextQuote);
	m_tagTextSample = "samp";
	m_tagLookup.insert(&m_tagTextSample, Document::TagTextSample);
	m_tagTextSpan = "span";
	m_tagLookup.insert(&m_tagTextSpan, Document::TagTextSpan);
	m_tagTextStrong = "strong";
	m_tagLookup.insert(&m_tagTextStrong, Document::TagTextStrong);
	m_tagTextSubscript = "sub";
	m_tagLookup.insert(&m_tagTextSubscript, Document::TagTextSubscript);
	m_tagTextSuperscript = "sup";
	m_tagLookup.insert(&m_tagTextSuperscript, Document::TagTextSuperscript);
	m_tagTextVariable = "var";
	m_tagLookup.insert(&m_tagTextVariable, Document::TagTextVariable);
}

Document::Tag XmlFragmentReaderHelper::tag(const QStringRef& name) const {
	return m_tagLookup.value(name);
}


class XmlFragmentReaderPrivate {
public:
	XmlFragmentReaderPrivate(XmlFragmentReader *reader)
	 : q_ptr (reader), errorCode(XmlFragmentReader::NoError)
	{ }
public:
	void readInline(Document::Tag tag);
	void readParagraph();
	void readHeading();
	void readSection();
	void readFragment();
	void error(XmlFragmentReader::ErrorCode code, const QString& text);
private:
	QXmlStreamReader* xml;
	XmlFragmentReader::ErrorCode errorCode;
	QString errorString;
	int errorLine;
	int errorColumn;
	FragmentBuilder builder;
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

void XmlFragmentReaderPrivate::readInline(Document::Tag tag) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	builder.startElement(tag);
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			builder.insertCharacters(xml->text().toString());
		} else if(xml->isStartElement()) {
			Document::Tag tag = helper->tag(xml->name());
			switch(tag) {
				case Document::TagTextAbbreviation:
				case Document::TagTextCode:
				case Document::TagTextDefinition:
				case Document::TagTextEmphasis:
				case Document::TagTextKeyboard:
				case Document::TagTextQuote:
				case Document::TagTextSample:
				case Document::TagTextSpan:
				case Document::TagTextStrong:
				case Document::TagTextSubscript:
				case Document::TagTextSuperscript:
				case Document::TagTextVariable:
					readInline(tag);
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else  if(xml->isEndElement()) {
			break;
		}
	}
	builder.endElement();
}

void XmlFragmentReaderPrivate::readHeading() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	builder.startElement(Document::TagHeading);
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			builder.insertCharacters(xml->text().toString());
		} else if(xml->isStartElement()) {
			Document::Tag tag = helper->tag(xml->name());
			switch(tag) {
				case Document::TagTextAbbreviation:
				case Document::TagTextCode:
				case Document::TagTextDefinition:
				case Document::TagTextEmphasis:
				case Document::TagTextKeyboard:
				case Document::TagTextQuote:
				case Document::TagTextSample:
				case Document::TagTextSpan:
				case Document::TagTextStrong:
				case Document::TagTextSubscript:
				case Document::TagTextSuperscript:
				case Document::TagTextVariable:
					readInline(tag);
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else if(xml->isEndElement()) {
			break;
		}
	}
	builder.endElement();
}

void XmlFragmentReaderPrivate::readParagraph() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	builder.startElement(Document::TagParagraph);
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			builder.insertCharacters(xml->text().toString());
		} else if(xml->isStartElement()) {
			Document::Tag tag = helper->tag(xml->name());
			switch(tag) {
				case Document::TagTextAbbreviation:
				case Document::TagTextCode:
				case Document::TagTextDefinition:
				case Document::TagTextEmphasis:
				case Document::TagTextKeyboard:
				case Document::TagTextQuote:
				case Document::TagTextSample:
				case Document::TagTextSpan:
				case Document::TagTextStrong:
				case Document::TagTextSubscript:
				case Document::TagTextSuperscript:
				case Document::TagTextVariable:
					readInline(tag);
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else  if(xml->isEndElement()) {
			break;
		}
	}
	builder.endElement();
}

void XmlFragmentReaderPrivate::readSection() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	builder.startElement(Document::TagSection);
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			switch(helper->tag(xml->name())) {
				case Document::TagSection: readSection(); break;
				case Document::TagHeading: readHeading(); break;
				case Document::TagParagraph: readParagraph(); break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else if(xml->isEndElement()) {
			break;
		}
	}
	builder.endElement();
}


void XmlFragmentReaderPrivate::readFragment() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	builder.startDocument();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			switch(helper->tag(xml->name())) {
				case Document::TagSection: readSection(); break;
				case Document::TagHeading: readHeading(); break;
				case Document::TagParagraph: readParagraph(); break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else if(xml->isEndElement()) {
			break;
		}
	}
	builder.endDocument();
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
	
	while(!d->xml->atEnd() && !d->errorCode) {
		d->xml->readNext();
		if(d->xml->isStartElement()) {
			if(d->xml->name() == "fragment") {
				d->readFragment();
			} else {
				d->error(UnallowedElement, QString("'%1' not allowed here").arg(d->xml->name().toString()));
			}
		}
	}
	if(d->errorCode) {
		delete d->builder.takeFragment();
	}
	return d->builder.fragment();
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
