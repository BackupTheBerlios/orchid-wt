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


#ifndef _ORCHID_FRAGMENTBUILDER_H_
#define _ORCHID_FRAGMENTBUILDER_H_

#include <flower/documentprocessor.h>

namespace Orchid {

class DomFragment;

class FragmentBuilderPrivate;
class FragmentBuilder : public DocumentProcessor {
public:
	FragmentBuilder();
	~FragmentBuilder();
public:
	void startDocument(const DocumentHead &head = DocumentHead());
	void endDocument();
	void startElement(Document::Tag tag);
	void endElement();
	void insertCharacters(const QString &str);
	void setAttribute(Document::Attribute attr, const QVariant &val);
public:
	DomFragment *fragment() const;
	DomFragment *takeFragment();
private:
	Q_DECLARE_PRIVATE(FragmentBuilder)
};

}

#endif
