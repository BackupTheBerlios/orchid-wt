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


#ifndef _ORCHID_XHTMLSTREAMWRITER_H_
#define _ORCHID_XHTMLSTREAMWRITER_H_

#include "documentprocessor.h"

class QXmlStreamWriter;
class QIODevice;

namespace Orchid {

class XHtml11StreamWriterPrivate;
class ORCHID_FLOWER_EXPORT XHtml11StreamWriter : public DocumentProcessor {
public:
	XHtml11StreamWriter(QIODevice* device = 0);
public:
	QXmlStreamWriter* xmlWriter();
	void setDevice(QIODevice* device);
public:
	void startDocument(const DocumentHead &head = DocumentHead());
	void endDocument();
	void startElement(Document::Tag special);
	void endElement();
	void insertCharacters(const QString& str);
	void setAttribute(Document::Attribute attr, const QVariant& val);
private:
	Q_DECLARE_PRIVATE(XHtml11StreamWriter)
};

}

#endif
