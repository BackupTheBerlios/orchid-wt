/*
 * This file is part of the Orchid libraries
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


#include "xmlfragmentreader.h"

#include <QtCore/QHash>
#include <QtXml/QXmlStreamReader>
#include "fragmentdom.h"
#include "fragmentbuilder.h"

#include <QtCore/QtDebug> // TODO rm this

namespace Orchid {
	
/**
 * \class XmlFragmentReader
 *
 * \brief The XmlFragmentReader class provides a reader for
 * reading xml-formatted fragments into a DocumentProcessor
 *
 * The XmlFragmentReader can be used to read xml-formatted fragments
 * and let them be processed by a DocumentProcessor. This can be
 * creating a DomFragment by using FragmentBuilder or convert it to
 * another format.
 *
 * \sa DocumentProcessor
 */

/**
 * \enum XmlFragmentReader::ErrorCode
 *
 * Possible error codes.
 */

/**
 *\var XmlFragmentReader::ErrorCode XmlFragmentReader::NoError
 * No error has occurred.
 */

/**
 * \var XmlFragmentReader::ErrorCode XmlFragmentReader::UnallowedElement
 * An element with the used tag wasn't allowed there.
 */

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
	XmlFragmentReaderPrivate(XmlFragmentReader *reader);
public:
	void readInline();
	void readParagraph();
	void readHeading();
	void readSection();
	void readFragment();
	void readBodyPart();
void error(XmlFragmentReader::ErrorCode code, const QString& text);
private:
	DocumentProcessor *dest;
	QIODevice *device;
	QXmlStreamReader *xml;
	QString string;
	XmlFragmentReader::ErrorCode errorCode;
	QString errorString;
	int errorLine;
	int errorColumn;
private:
	XmlFragmentReader* q_ptr;
	Q_DECLARE_PUBLIC(XmlFragmentReader);
};

XmlFragmentReaderPrivate::XmlFragmentReaderPrivate(XmlFragmentReader *reader)
	 : q_ptr(reader), errorCode(XmlFragmentReader::NoError)
{
	dest = 0;
	device = 0;
	xml = 0;
}

void XmlFragmentReaderPrivate::error(XmlFragmentReader::ErrorCode code, const QString& text) {
	errorCode = code;
	errorString = text;
	errorLine = xml->lineNumber();
	errorColumn = xml->columnNumber();
}

void XmlFragmentReaderPrivate::readInline() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			dest->insertCharacters(xml->text().toString());
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
					dest->startElement(tag);
					readInline();
					dest->endElement();
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else  if(xml->isEndElement()) {
			break;
		}
	}
}

void XmlFragmentReaderPrivate::readHeading() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	dest->startElement(Document::TagHeading);
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			dest->insertCharacters(xml->text().toString());
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
					dest->startElement(tag);
					readInline();
					dest->endElement();
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else if(xml->isEndElement()) {
			break;
		}
	}
	dest->endElement();
}

void XmlFragmentReaderPrivate::readParagraph() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	dest->startElement(Document::TagParagraph);
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isCharacters()) {
			dest->insertCharacters(xml->text().toString());
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
					dest->startElement(tag);
					readInline();
					dest->endElement();
					break;
				default:
					error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
					break;
			}
		} else  if(xml->isEndElement()) {
			break;
		}
	}
	dest->endElement();
}

void XmlFragmentReaderPrivate::readSection() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	dest->startElement(Document::TagSection);
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
	dest->endElement();
}


void XmlFragmentReaderPrivate::readBodyPart() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
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
}

void XmlFragmentReaderPrivate::readFragment() {
	// NOTE the format of Fragment files will change and
	// this function will be required
	readBodyPart();
}

/**
 * Constructs a fragment reader.
 */
XmlFragmentReader::XmlFragmentReader() {
	d_ptr = new XmlFragmentReaderPrivate(this);
}

/**
 * Constructs a fragment reader that reads from \a xml into \a processor.
 *
 * \sa setXmlStreamReader()
 */
XmlFragmentReader::XmlFragmentReader(DocumentProcessor *processor, QXmlStreamReader *xml) {
	d_ptr = new XmlFragmentReaderPrivate(this);
	Q_D(XmlFragmentReader);
	d->dest = processor;
	d->xml = xml;
}

/**
 * Constructs a fragment reader that reads from \a device into \a processor.
 *
 * \sa setDevice()
 */
XmlFragmentReader::XmlFragmentReader(DocumentProcessor *processor, QIODevice *device) {
	d_ptr = new XmlFragmentReaderPrivate(this);
	Q_D(XmlFragmentReader);
	d->dest = processor;
	d->device = device;
	d->xml = new QXmlStreamReader(device);
}

/**
 * Constructs a fragment reader that reads from \a str into \a processor.
 *
 * \note If you want to use readBody() the content needs to
 * enclosed into a \<body\>-tag. If you want to use
 * readInline() enclose the content into a \<inline\>-tag.
 *
 * \sa setString(), setBodyString()
 */
XmlFragmentReader::XmlFragmentReader(DocumentProcessor *processor, const QString &str) {
	d_ptr = new XmlFragmentReaderPrivate(this);
	Q_D(XmlFragmentReader);
	d->dest = processor;
	d->string = str;
	d->xml = new QXmlStreamReader(str);
}

/**
 * Destructs the reader.
 */
XmlFragmentReader::~XmlFragmentReader() {
	Q_D(XmlFragmentReader);
	if(d->device) delete d->xml;
	delete d_ptr;
}

/**
 * Returns the processor used by the reader for further processing.
 */
DocumentProcessor *XmlFragmentReader::processor() const {
	Q_D(const XmlFragmentReader);
	return d->dest;
}

/**
 * Sets the processor for for further processing to \a processor.
 */
void XmlFragmentReader::setProcessor(DocumentProcessor *processor) {
	Q_D(XmlFragmentReader);
	d->dest = processor;
}

/**
 * Returns the device used for reading from, or 0 if no device was set.
 *
 * \sa setDevice(), string(), xmlStreamReader()
 */
QIODevice *XmlFragmentReader::device() const {
	Q_D(const XmlFragmentReader);
	return d->device;
}

/**
 * Sets the device used for reading from to \a device.
 *
 * \sa device(), setString(), setXmlStreamReader()
 */
void XmlFragmentReader::setDevice(QIODevice *device) {
	Q_D(XmlFragmentReader);
	if(d->device || !d->string.isEmpty()) {
		d->string.clear();
		d->xml->setDevice(device);
    } else {
		d->xml = new QXmlStreamReader(device);
	}
	d->device = device;
}

/**
 * Returns the string used for reading from, or QString() if none was set.
 *
 * \sa setString(), device(), xmlStreamReader()
 */
QString XmlFragmentReader::string() const {
	Q_D(const XmlFragmentReader);
	return d->string;
}

/**
 * Sets the string used for reading from to \a str.
 *
 * \note If you want to use \a str together with readBody()
 * or readInline() you might find setBodyString() or
 * setInlineString() more useful.
 *
 * \sa string(), setBodyString(), setInlineString(),
 * setDevice(), setXmlStreamReader()
 */
void XmlFragmentReader::setString(const QString &str) {
	Q_D(XmlFragmentReader);
	if(d->device || !d->string.isEmpty()) {
		d->device = 0;
		d->xml->clear();
		d->xml->addData(str);
	} else {
		d->xml = new QXmlStreamReader(str);
	}
	d->string = str;
}

/**
 * Sets the string used for reading from to \a str.
 * Encloses the content of \a str into a \<body\>-tag
 * that is needed by readBody().
 *
 * \note If you want to use \a str with readDocument()
 * or readInline you might find setString() or
 * setInlineString() more useful.
 *
 * \sa string(), setString(), setInlineString(), setDevice(),
 * setXmlStreamReader()
 */
void XmlFragmentReader::setBodyString(const QString &str) {
	Q_D(XmlFragmentReader);
	if(d->device || !d->string.isEmpty()) {
		d->device = 0;
		d->xml->clear();
	} else {
		d->xml = new QXmlStreamReader();
	}
	d->xml->addData("<body>");
	d->xml->addData(str);
	d->xml->addData("</body>");
	d->string = str;
}

/**
 * Sets the string used for reading from to \a str.
 * Encloses the content of \a str into a \<inline\>-tag
 * that is needed by readInline().
 *
 * \note If you want to use \a str with readDocument() or
 * readBody() you might find setString() or setBodyString()
 * more useful.
 *
 * \sa string(), setString(), setBodyString(), setDevice(),
 * setXmlStreamReader()
 */
void XmlFragmentReader::setInlineString(const QString &str) {
	Q_D(XmlFragmentReader);
	if(d->device || !d->string.isEmpty()) {
		d->device = 0;
		d->xml->clear();
	} else {
		d->xml = new QXmlStreamReader();
	}
	d->xml->addData("<inline>");
	d->xml->addData(str);
	d->xml->addData("</inline>");
	d->string = str;
}

/**
 * Returns the stream reader used for reading from, or 0 if none was set.
 *
 * \sa setXmlStreamReader(), device(), string()
 */
QXmlStreamReader *XmlFragmentReader::xmlStreamReader() const {
	Q_D(const XmlFragmentReader);
	return (d->device || !d->string.isEmpty()) ? 0 : d->xml;
}

/**
 * Sets the stream reader used for reading from to \a xml.
 *
 * \sa xmlStreamReader(), setDevice(), setString()
 */
void XmlFragmentReader::setXmlStreamReader(QXmlStreamReader *xml) {
	Q_D(XmlFragmentReader);
	if(d->device || !d->string.isEmpty()) delete d->xml;
	d->device = 0;
	d->string.clear();
	d->xml = xml;
}

/**
 * Reads a full document and forwards it to the processor.
 *
 * \sa readBody(), readInline()
 */
bool XmlFragmentReader::readDocument() {
	Q_D(XmlFragmentReader);
	if(!(d->dest && d->xml)) return false;
	
	while(!d->xml->atEnd() && !d->errorCode) {
		d->xml->readNext();
		if(d->xml->isStartElement()) {
			if(d->xml->name() == "fragment") {
				d->dest->startDocument();
				d->readFragment();
				d->dest->endDocument();
			} else {
				d->error(UnallowedElement, QString("'%1' not allowed here").arg(d->xml->name().toString()));
			}
		}
	}
	if(d->errorCode) {
		return false;
	}
	return true;
}

/**
 * Reads the body part of a document and forwards it to the
 * processor. The content needs to be enclosed by a
 * \<body\>-tag.
 *
 * \sa readDocument(), readInline()
 */
bool XmlFragmentReader::readBody() {
	Q_D(XmlFragmentReader);
	while(!d->xml->atEnd() && !d->errorCode) {
		d->xml->readNext();
		if(d->xml->isStartElement()) {
			if(d->xml->name() == "body") {
				d->readBodyPart();
			} else {
				d->error(UnallowedElement, QString("'%1' not allowed here").arg(d->xml->name().toString()));
			}
		}
	}
	if(d->errorCode) {
		return false;
	}
	return false;
}

/**
 * Reads an inline part of a document and forwards it to the
 * processor. The content needs to be enclosed by a
 * \<inline\>-tag.
 *
 * \sa readDocument(), readBody()
 */
bool XmlFragmentReader::readInline() {
	Q_D(XmlFragmentReader);
	while(!d->xml->atEnd() && !d->errorCode) {
		d->xml->readNext();
		if(d->xml->isStartElement()) {
			if(d->xml->name() == "inline") {
				d->readInline();
			} else {
				d->error(UnallowedElement, QString("'%1' not allowed here").arg(d->xml->name().toString()));
			}
		}
	}
	if(d->errorCode) {
		return false;
	}
	return false;
}

/**
 * Returns the code of the last error or XmlFragmentReader::NoError
 * if no error occured.
 *
 * \sa errorString(), errorLine(), errorColumn()
 */
XmlFragmentReader::ErrorCode XmlFragmentReader::errorCode() const {
	Q_D(const XmlFragmentReader);
	return d->errorCode;
}

/**
 * Returns the message of the last error.
 *
 * \sa errorCode(), errorLine(), errorColumn()
 */
QString XmlFragmentReader::errorString() const {
	Q_D(const XmlFragmentReader);
	return d->errorString;
}

/**
 * Returns the line where the last error occured, starting with 1.
 *
 * \sa errorCode(), errorString(), errorColumn()
 */
int XmlFragmentReader::errorLine() const {
	Q_D(const XmlFragmentReader);
	return d->errorLine;
}

/**
 * Returns the column where the last error occured, starting with 0.
 *
 * \sa errorCode(), errorString(), errorLine()
 */
int XmlFragmentReader::errorColumn() const {
	Q_D(const XmlFragmentReader);
	return d->errorColumn;
}

}
