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


#ifndef _ORCHID_DOCUMENTPROCESSOR_H_
#define _ORCHID_DOCUMENTPROCESSOR_H_

#include <QtGlobal>
#include "documenthead.h"
#include "documentenums.h"

class QString;
class QVariant;

namespace Orchid {
	
class StyleAttributes;
class Style;

class DocumentProcessorPrivate;
class ORCHID_FLOWER_EXPORT DocumentProcessor {
public:
	DocumentProcessor();
	virtual ~DocumentProcessor();
public:
	StyleAttributes attributes(const Style* style);
	void regStyle(const Style* style, const QString& prefix);
	static QString defaultRoleName(Document::Role role);
public:
	virtual void startDocument(const DocumentHead &head = DocumentHead()) = 0;
	virtual void endDocument() = 0;
	virtual void startElement(Document::Tag tag) = 0;
	virtual void endElement() = 0;
	virtual void insertCharacters(const QString& str) = 0;
	virtual void setAttribute(Document::Attribute attr, const QVariant& val) = 0;
protected:
	DocumentProcessor(DocumentProcessorPrivate* dd);
private:
	Q_DECLARE_PRIVATE(DocumentProcessor)
protected:
	DocumentProcessorPrivate* d_ptr;
};

}

#endif
