#include "cppfragmentwriter.h" 

#include <QtCore/QTextStream>
#include <QtCore/QHash>
#include <QtCore/QDateTime>

#include <QtCore/QtDebug>

#include <globals.h>
#include "fragmentdom.h"

namespace Orchid {

class CppFragmentWriterHelper {
private:
	CppFragmentWriterHelper();
public:
	static CppFragmentWriterHelper* inst();
public:
	QString enumName(HtmlTag tag);
private:
	QVector<QString> m_lookup;
};

CppFragmentWriterHelper* CppFragmentWriterHelper::inst() {
	static CppFragmentWriterHelper helper;
	return &helper;
}

CppFragmentWriterHelper::CppFragmentWriterHelper() {
	m_lookup.resize(HtmlTagCount);
	m_lookup[HtmlTagSection] = "HtmlTagSection";
	m_lookup[HtmlTagHeading] = "HtmlTagHeading";
	m_lookup[HtmlTagParagraph] = "HtmlTagParagraph";
	m_lookup[HtmlTagTextAbbreviation] = "HtmlTagTextAbbreviation";
	m_lookup[HtmlTagTextCode] = "HtmlTagTextCode";
	m_lookup[HtmlTagTextDefinition] = "HtmlTagTextDefinition";
	m_lookup[HtmlTagTextEmphasis] = "HtmlTagTextEmphasis";
	m_lookup[HtmlTagTextKeyboard] = "HtmlTagTextKeyboard";
	m_lookup[HtmlTagTextQuote] = "HtmlTagTextQuote";
	m_lookup[HtmlTagTextSample] = "HtmlTagTextSample";
	m_lookup[HtmlTagTextSpan] = "HtmlTagTextSpan";
	m_lookup[HtmlTagTextStrong] = "HtmlTagTextStrong";
	m_lookup[HtmlTagTextSubscript] = "HtmlTagTextSubscript";
	m_lookup[HtmlTagTextSuperscript] = "HtmlTagTextSuperscript";
	m_lookup[HtmlTagTextVariable] = "HtmlTagTextVariable";
}

QString CppFragmentWriterHelper::enumName(HtmlTag tag) {
	return m_lookup[tag];
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
			*stream << "\t\twriter->writeStartElement("<<helper->enumName(element->tag())<<");\n";
			break;
	}
	foreach(DomNode* child, element->childs()) {
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				*stream << "\t\twriter->writeCharacters(\""<< chars->text()<<"\");\n";
			} break;
			default: {
				DomElement* childElement = dynamic_cast<DomElement*>(child);
				if(childElement) writeElement(childElement);
			} break;
		}
	}
	switch(element->tag()) {
		default:
			*stream << "\t\twriter->writeEndElement();\n";
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
		<< "\tvoid build(HtmlStreamWriter* writer) {\n";
	foreach(DomNode* child, fragment->childs()) {
		switch(child->type()) {
			case DomUnknownType: break;
			case DomPCDATAType: {
				DomCharacters* chars = static_cast<DomCharacters*>(child);
				*d->stream << "\t\twriter->writeCharacters("<< chars->text()<<");\n";
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
