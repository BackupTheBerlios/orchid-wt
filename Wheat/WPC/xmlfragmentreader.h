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


#ifndef _ORCHID_XMLFRAGMENTREADER_H_
#define _ORCHID_XMLFRAGMENTREADER_H_

#include <QtCore/QtGlobal>

class QXmlStreamReader;
class QIODevice;

namespace Orchid {

class DocumentProcessor;

class XmlFragmentReaderPrivate;
class XmlFragmentReader {
public:
	enum ErrorCode {
		NoError = 0,
		UnallowedElement,
	};
public:
	XmlFragmentReader();
	XmlFragmentReader(DocumentProcessor *dest, QXmlStreamReader* xml = 0);
	XmlFragmentReader(DocumentProcessor *dest, QIODevice *device);
	~XmlFragmentReader();
public:
	DocumentProcessor *processor() const;
	void setProcessor(DocumentProcessor *processor);
	QIODevice *device() const;
	void setDevice(QIODevice *device);
	QXmlStreamReader *xmlStreamReader() const;
	void setXmlStreamReader(QXmlStreamReader *xml);
	bool readBody();
	bool readDocument();
	ErrorCode errorCode() const;
	QString errorString() const;
	int errorLine() const;
	int errorColumn() const;
private:
	XmlFragmentReaderPrivate* d_ptr;
	Q_DECLARE_PRIVATE(XmlFragmentReader);
};

}

#endif
