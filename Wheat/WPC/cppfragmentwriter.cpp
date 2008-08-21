#include "cppfragmentwriter.h" 

#include <QtCore/QTextStream>
#include <QtCore/QHash>
#include <QtCore/QDateTime>

#include <QtCore/QtDebug>

#include <globals.h>
#include "fragmentdom.h"

namespace Orchid {

class CppFragmentWriterPrivate {
public:
	CppFragmentWriterPrivate(CppFragmentWriter* writer);
	void writeElement(DomElement* elemnt);
	static QString enumName(HtmlTag tag);
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
	switch(element->tag()) {
		default:
			*stream << "\t\twriter->writeBeginSpecial("<<enumName(element->tag())<<");\n";
			break;
	}
	foreach(DomNode* child, element->childs()) {
		DomElement* childElement = dynamic_cast<DomElement*>(child);
		if(childElement) writeElement(childElement);
	}
	switch(element->tag()) {
		default:
			*stream << "\t\twriter->writeEndSpecial();\n";
			break;
	}
}

QString CppFragmentWriterPrivate::enumName(HtmlTag tag) {
	switch(tag) {
		case HtmlTagSection: return "HtmlTagSection";
		case HtmlTagHeading: return "HtmlTagHeading";
		case HtmlTagParagraph: return "HtmlTagParagraph";
		case HtmlTagTextCode: return "HtmlTagTextCode";
		case HtmlTagTextAbbreviation: return "HtmlTagTextAbbreviation";
		case HtmlTagTextDefinition: return "HtmlTagTextDefinition";
		case HtmlTagTextEmphasis: return "HtmlTagTextEmphasis";
		case HtmlTagTextKeyboard: return "HtmlTagTextKeyboard";
		case HtmlTagTextQuote: return "HtmlTagTextQuote";
		case HtmlTagTextSample: return "HtmlTagTextSample";
		case HtmlTagTextSpan: return "HtmlTagTextSpan";
		case HtmlTagTextStrong: return "HtmlTagTextStrong";
		case HtmlTagTextSubscript: return "HtmlTagTextSubscript";
		case HtmlTagTextSuperscript: return "HtmlTagTextSuperscript";
		case HtmlTagTextVariable: return "HtmlTagTextVariable";
		default: 
			qDebug() << "unknown tag" << tag;
			Q_ASSERT(false && "enumName not doesn't handle this enum");
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
		DomElement* childElement = dynamic_cast<DomElement*>(child);
		if(childElement) d->writeElement(childElement);
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
		<< " * This file is generated from " << input.leftJustified(42) << "  *\n"
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
