#include "xmlfragmentreader.h"

#include <QtXml/QXmlStreamReader>
#include "fragmentdom.h"

namespace Orchid {

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
		if(xml->isStartElement()) {
			{
				error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
			}
		} else if(xml->isEndElement()) {
			return;
		}
	}
}

void XmlFragmentReaderPrivate::readSection(DomSection* section, QXmlStreamReader* xml) {
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			if(xml->name() == "section") {
				DomSection* sub = new DomSection();
				readSection(sub, xml);
				section->append(sub);
			} else if(xml->name() == "h") {
				DomHeading* heading = new DomHeading();
				readHeading(heading, xml);
				section->append(heading);
			} else {
				error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
			}
		} else if(xml->isEndElement()) {
			return;
		}
	}
}


void XmlFragmentReaderPrivate::readFragment(DomFragment* fragment, QXmlStreamReader* xml) {
	while(!xml->atEnd() && !errorCode) {
		xml->readNext();
		if(xml->isStartElement()) {
			if(xml->name() == "section") {
				DomSection* section = new DomSection();
				readSection(section, xml);
				fragment->append(section);
			} else if(xml->name() == "h") {
				DomHeading* heading = new DomHeading();
				readHeading(heading, xml);
				fragment->append(heading);
			} else {
				error(XmlFragmentReader::UnallowedElement, QString("'%1' not allowed here").arg(xml->name().toString()));
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
