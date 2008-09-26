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


#ifndef _ORCHID_XMLFRAGMENTWRITER_H_
#define _ORCHID_XMLFRAGMENTWRITER_H_

#include <QtCore/QtGlobal>

class QXmlStreamWriter;

namespace Orchid {

class DomFragment;

class XmlFragmentWriterPrivate;
class XmlFragmentWriter {
public:
	XmlFragmentWriter();
	~XmlFragmentWriter();
public:
	bool write(QXmlStreamWriter* xml, DomFragment* fragment);
protected:
	XmlFragmentWriterPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(XmlFragmentWriter)
};

}

#endif
