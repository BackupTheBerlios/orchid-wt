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
	void readInline(Document::Tag tag);
	void readParagraph();
	void readHeading();
	void readSection();
	void readFragment();
	void error(XmlFragmentReader::ErrorCode code, const QString& text);
private:
	DocumentProcessor *dest;
	QIODevice *device;
	QXmlStreamReader *xml;
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

void XmlFragmentReaderPrivate::readInline(Document::Tag tag) {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	dest->startElement(tag);
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
	dest->endElement();
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


void XmlFragmentReaderPrivate::readFragment() {
	XmlFragmentReaderHelper* helper = XmlFragmentReaderHelper::inst();
	dest->startDocument();
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
	dest->endDocument();
}


/**
 * Constructs a fragment reader.
 */
XmlFragmentReader::XmlFragmentReader() {
	d_ptr = new XmlFragmentReaderPrivate(this);
}

/**
 * Constructs a fragment reader that reads from \a xml into \a processor.
 */
XmlFragmentReader::XmlFragmentReader(DocumentProcessor *processor, QXmlStreamReader *xml) {
	d_ptr = new XmlFragmentReaderPrivate(this);
	Q_D(XmlFragmentReader);
	d->dest = processor;
	d->xml = xml;
}

/**
 * Constructs a fragment reader that reads from \a device into \a processor.
 */
XmlFragmentReader::XmlFragmentReader(DocumentProcessor *processor, QIODevice *device) {
	d_ptr = new XmlFragmentReaderPrivate(this);
	Q_D(XmlFragmentReader);
	d->dest = processor;
	d->device = device;
	d->xml = new QXmlStreamReader(device);
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
 * \sa setDevice(), xmlStreamReader()
 */
QIODevice *XmlFragmentReader::device() const {
	Q_D(const XmlFragmentReader);
	return d->device;
}

/**
 * Sets the device used for reading from to \a device.
 *
 * \sa device(), setXmlStreamReader()
 */
void XmlFragmentReader::setDevice(QIODevice *device) {
	Q_D(XmlFragmentReader);
	if(d->device) delete d->xml;
	d->device = device;
	d->xml = new QXmlStreamReader(device);
}

/**
 * Returns the stream reader used for reading from, or 0 if none was set.
 *
 * \sa setXmlStreamReader(), device()
 */
QXmlStreamReader *XmlFragmentReader::xmlStreamReader() const {
	Q_D(const XmlFragmentReader);
	return d->device ? 0 : d->xml;
}

/**
 * Sets the stream reader used for reading from to \a xml.
 *
 * \sa xmlStreamReader(), setDevice()
 */
void XmlFragmentReader::setXmlStreamReader(QXmlStreamReader *xml) {
	Q_D(XmlFragmentReader);
	if(d->device) delete d->xml;
	d->device = 0;
	d->xml = xml;
}

/**
 * Reads a full document and returns it to the processor.
 *
 * \sa readBody()
 */
bool XmlFragmentReader::readDocument() {
	Q_D(XmlFragmentReader);
	if(!(d->dest && d->xml)) return false;
	
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
		return false;
	}
	return true;
}

/**
 * Reads the body of a document and returns it to the processor.
 *
 * \note not implemented yet
 *
 * \sa readDocument()
 */
bool XmlFragmentReader::readBody() {
	// TODO might need to be changed
// 	d->readFragment();
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
