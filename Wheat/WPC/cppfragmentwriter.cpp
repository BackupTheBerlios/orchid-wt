/*
 * This file is part of the Orchid tools applications
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "cppfragmentwriter.h" 

#include <QtCore/QTextStream>
#include <QtCore/QHash>
#include <QtCore/QDateTime>

#include <QtCore/QtDebug>

#include <globals.h>
#include <flower/fragmentdom.h>

namespace Orchid {

class CppFragmentWriterHelper {
private:
	CppFragmentWriterHelper();
public:
	static CppFragmentWriterHelper* inst();
public:
	QString enumName(Document::Tag tag);
	static QString escapeCString(const QString& str);
private:
	QVector<QString> m_lookup;
};

CppFragmentWriterHelper* CppFragmentWriterHelper::inst() {
	static CppFragmentWriterHelper helper;
	return &helper;
}

CppFragmentWriterHelper::CppFragmentWriterHelper() {
	m_lookup.resize(Document::TagCount);
	m_lookup[Document::TagSection] = "TagSection";
	m_lookup[Document::TagHeading] = "TagHeading";
	m_lookup[Document::TagParagraph] = "TagParagraph";
	m_lookup[Document::TagTextAbbreviation] = "TagTextAbbreviation";
	m_lookup[Document::TagTextCode] = "TagTextCode";
	m_lookup[Document::TagTextDefinition] = "TagTextDefinition";
	m_lookup[Document::TagTextEmphasis] = "TagTextEmphasis";
	m_lookup[Document::TagTextKeyboard] = "TagTextKeyboard";
	m_lookup[Document::TagTextQuote] = "TagTextQuote";
	m_lookup[Document::TagTextSample] = "TagTextSample";
	m_lookup[Document::TagTextSpan] = "TagTextSpan";
	m_lookup[Document::TagTextStrong] = "TagTextStrong";
	m_lookup[Document::TagTextSubscript] = "TagTextSubscript";
	m_lookup[Document::TagTextSuperscript] = "TagTextSuperscript";
	m_lookup[Document::TagTextVariable] = "TagTextVariable";
}

QString CppFragmentWriterHelper::enumName(Document::Tag tag) {
	return m_lookup[tag];
}

QString CppFragmentWriterHelper::escapeCString(const QString& str) {
	QString newStr;
	
	QByteArray bytes = str.toUtf8();
	// the new string requires at least bytes.size() bytes
	newStr.reserve((bytes.size()));
	
	for(int i = 0; i < bytes.size(); ++i) {
		switch((unsigned char)bytes[i]) {
			case '\\': newStr += "\\\\"; break;
			case '\"': newStr += "\\\""; break;
			case '\t': newStr += "\\t"; break;
			case '\n': newStr += "\\n"; break;
			case 1 ... 8:
			case 11 ... 31:
			case 128 ... 255:
				(newStr += "\\x") += QString::number((unsigned char)bytes[i], 16);
				break;
			default: newStr += bytes.at(i); break;
		}
	}
	return newStr;
}


class CppFragmentWriterPrivate {
public:
	CppFragmentWriterPrivate(CppFragmentWriter* writer);
	void writeElement(DomElement* elemnt);
protected:
	CppFragmentWriter* q_ptr;
private:
	Q_DECLARE_PUBLIC(CppFragmentWriter)
	QTextStream* stream;
};

CppFragmentWriterPrivate::CppFragmentWriterPrivate(CppFragmentWriter* writer) {
	q_ptr = writer;
}

void CppFragmentWriterPrivate::writeElement(DomElement* element) {
	CppFragmentWriterHelper* helper = CppFragmentWriterHelper::inst();
	switch(element->tag()) {
		default:
			*stream << "\t\twriter->startElement("<<helper->enumName(element->tag())<<");\n";
			break;
	}
	foreach(DomNode* child, element->childs()) {
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				QString escaped = CppFragmentWriterHelper::escapeCString(chars->text());
				*stream << "\t\twriter->insertCharacters(QString::fromUtf8(\"" << escaped << "\"));\n";
			} break;
			default: {
				DomElement* childElement = dynamic_cast<DomElement*>(child);
				if(childElement) writeElement(childElement);
			} break;
		}
	}
	switch(element->tag()) {
		default:
			*stream << "\t\twriter->endElement();\n";
			break;
	}
}

CppFragmentWriter::CppFragmentWriter(QTextStream* stream) {
	d_ptr = new CppFragmentWriterPrivate(this);
	Q_D(CppFragmentWriter);
	d->stream = stream;
}

CppFragmentWriter::~CppFragmentWriter() {
	delete d_ptr;
}

void CppFragmentWriter::write(DomFragment* fragment) {
	Q_D(CppFragmentWriter);

	QString classname("Test");

	*d->stream
		<< "class " << classname << " : public Orchid::Fragment {\n"
		<< "public:\n"
		<< "\tvoid build(DocumentProcessor* writer) {\n"
		<< "\t\tusing namespace ::Orchid::Document;\n";
	foreach(DomNode* child, fragment->childs()) {
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				QString escaped = CppFragmentWriterHelper::escapeCString(chars->text());
				*d->stream << "\t\twriter->insertCharacters(QString::fromUtf8(\"" << escaped << "\"));\n";
			} break;
			default: {
				DomElement* childElement = dynamic_cast<DomElement*>(child);
				if(childElement) d->writeElement(childElement);
			} break;
		}
	}
	*d->stream
		<< "\t}\n"
		<< "};\n\n";
}

class CppHeaderWriterPrivate {
public:
	CppHeaderWriterPrivate(CppHeaderWriter* writer);
public:
	void writeCopyrightHeader();
	void writeHeaderProtectionStart();
	void writeHeaderProtectionEnd();
protected:
	CppHeaderWriter* q_ptr;
private:
	Q_DECLARE_PUBLIC(CppHeaderWriter)
	QTextStream* stream;
	QHash<QString, DomFragment*> fragments;
};

CppHeaderWriterPrivate::CppHeaderWriterPrivate(CppHeaderWriter* writer) {
	q_ptr = writer;
}

void CppHeaderWriterPrivate::writeCopyrightHeader() {
	QString input("test.txt");
	QString version(ORCHID_VERSION_STR);

	*stream << "/***************************************************************************\n"
		<< " * This file was generated from " << input.leftJustified(41) << "  *\n"
		<< " *                                                                         *\n"
		<< " * Generator: Orchid Web Fragment Compiler " << version.leftJustified(30) << "  *\n"
		<< " * Date:      " << QDateTime::currentDateTime().toString().leftJustified(59) << "  *\n"
		<< " *                                                                         *\n"
		<< " * WARNING!   ALL CHANGES TO THIS FILE WILL BE LOST                        *\n"
		<< " *                                                                         *\n"
		<< " ***************************************************************************/\n\n";
}

void CppHeaderWriterPrivate::writeHeaderProtectionStart() {
	QString headername("Test");
	*stream << "#ifndef _" << headername << "_H_\n"
		<< "#define _" << headername << "_H_\n\n";
}

void CppHeaderWriterPrivate::writeHeaderProtectionEnd() {
	QString headername("Test");
	*stream << "#endif // _" << headername << "_H_\n";
}


CppHeaderWriter::CppHeaderWriter(QTextStream* stream) {
	d_ptr = new CppHeaderWriterPrivate(this);
	Q_D(CppHeaderWriter);
	d->stream = stream;
}

CppHeaderWriter::~CppHeaderWriter() {
	delete d_ptr;
}

bool CppHeaderWriter::addFragment(DomFragment* fragment) {
	Q_D(CppHeaderWriter);
	if(d->fragments.contains("Fragment"))
		return false;
	d->fragments.insert("Fragment", fragment);
	return true;
}

void CppHeaderWriter::write() {
	Q_D(CppHeaderWriter);
	d->writeCopyrightHeader();
	d->writeHeaderProtectionStart();
	
	CppFragmentWriter writer(d->stream);
	foreach(DomFragment* frag, d->fragments) {
		writer.write(frag);
	}
	
	d->writeHeaderProtectionEnd();
}

}
