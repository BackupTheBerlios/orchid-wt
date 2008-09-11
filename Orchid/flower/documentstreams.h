#ifndef _ORCHID_DOCUMENTSTREAMS_H_
#define _ORCHID_DOCUMENTSTREAMS_H_

#include <QtCore/QString>
#include "documentprocessor.h"
#include <QtCore/QVariant>

namespace Orchid {

namespace Document {

class role;
class id;
class classname;
class language;

class heading;
class paragraph;

class abbreviation;

class InlineStream;
class BlockStream {
public:
	inline BlockStream(DocumentProcessor* writer);
public:
	inline DocumentProcessor* writer() const;
	inline BlockStream& operator<<(BlockStream&(*)(BlockStream&));
	inline InlineStream heading();
	inline InlineStream paragraph();
	inline InlineStream text();
	inline BlockStream& operator<<(const ::Orchid::Document::heading& obj);
	inline BlockStream& operator<<(const ::Orchid::Document::paragraph& obj);
	inline BlockStream& operator<<(const role& role);
	inline BlockStream& operator<<(const id& id);
	inline BlockStream& operator<<(const classname& name);
	inline BlockStream& operator<<(const language& lang);
private:
	DocumentProcessor* m_writer;
};

inline BlockStream::BlockStream(DocumentProcessor* writer)
	: m_writer(writer) {}

inline DocumentProcessor* BlockStream::writer() const { return m_writer; }
inline BlockStream& BlockStream::operator<<(BlockStream&(*fp)(BlockStream&))
{ return fp(*this); }


class TextManip;
class InlineStream {
	friend class BlockStream;
public:
	inline InlineStream(DocumentProcessor* writer);
	inline InlineStream(InlineStream& other);
	inline ~InlineStream();
public:
	inline DocumentProcessor* writer() const;
	inline InlineStream& operator<<(InlineStream&(*)(InlineStream&));
	inline InlineStream& operator<<(QChar c);
	inline InlineStream& operator<<(const QString& str);
	inline InlineStream& operator<<(const TextManip&);
	inline InlineStream& operator<<(const role& role);
	inline InlineStream& operator<<(const id& id);
	inline InlineStream& operator<<(const abbreviation& abbr);
	inline InlineStream& operator<<(const classname& name);
	inline InlineStream& operator<<(const language& lang);
private:
	inline InlineStream(BlockStream& block);
	Q_DISABLE_COPY(InlineStream);
// 	InlineStream& operator=(const InlineStream& other); 
private:
	DocumentProcessor* m_writer;
	bool m_inBlock;
};

inline InlineStream::InlineStream(DocumentProcessor* writer)
	: m_writer(writer), m_inBlock(false) { }
inline InlineStream::InlineStream(InlineStream& other)
	: m_writer(other.writer()), m_inBlock(other.m_inBlock)
{ other.m_inBlock = false; }
inline InlineStream::InlineStream(BlockStream& block)
	: m_writer(block.writer()), m_inBlock(true) { }
inline InlineStream::~InlineStream() 
{ if(m_inBlock) m_writer->writeEndElement(); }
	
inline DocumentProcessor* InlineStream::writer() const { return m_writer; }
inline InlineStream& InlineStream::operator<<(QChar c)
{ m_writer->writeCharacters(c); return *this; }
inline InlineStream& InlineStream::operator<<(const QString& str)
{ m_writer->writeCharacters(str); return *this; }
inline InlineStream& InlineStream::operator<<(InlineStream&(*fp)(InlineStream&))
{ return fp(*this); }

inline InlineStream BlockStream::heading()
{ m_writer->writeStartElement(TagHeading); return InlineStream(*this); }
inline InlineStream BlockStream::paragraph()
{ m_writer->writeStartElement(TagParagraph); return InlineStream(*this); }
inline InlineStream BlockStream::text()
{ return InlineStream(m_writer); }


// tags
BlockStream& section(BlockStream& s);
// end
BlockStream& end(BlockStream& s);

// tags
InlineStream& code(InlineStream& s);
InlineStream& definition(InlineStream& s);
InlineStream& emphasis(InlineStream& s);
InlineStream& keyboard(InlineStream& s);
InlineStream& quote(InlineStream& s);
InlineStream& sample(InlineStream& s);
InlineStream& span(InlineStream& s);
InlineStream& strong(InlineStream& s);
InlineStream& subscript(InlineStream& s);
InlineStream& superscript(InlineStream& s);
InlineStream& variable(InlineStream& s);
// end
InlineStream& end(InlineStream& s);

// class TextManip {
// public:
// 	virtual InlineStream& apply(InlineStream&) = 0;
// };

class role {
public:
	inline role(Role val) { this->val = val; }
	inline BlockStream& apply(BlockStream& s) const {
		s.writer()->setAttribute(AttributeRole, val);
		return s;
	}
	inline InlineStream& apply(InlineStream& s) const {
		s.writer()->setAttribute(AttributeRole, val);
		return s;
	}
private:
	Role val;
};

class id {
public:
	inline id(const QString val) { this->val = val; }
	inline BlockStream& apply(BlockStream& s) const {
		s.writer()->setAttribute(AttributeId, val);
		return s;
	}
	inline InlineStream& apply(InlineStream& s) const {
		s.writer()->setAttribute(AttributeId, val);
		return s;
	}
private:
	QString val;
};

class classname {
public:
	inline classname(const QString val) { this->val = val; }
	inline BlockStream& apply(BlockStream& s) const {
		s.writer()->setAttribute(AttributeClassname, val);
		return s;
	}
	inline InlineStream& apply(InlineStream& s) const {
		s.writer()->setAttribute(AttributeClassname, val);
		return s;
	}
private:
	QString val;
};

class language {
public:
	inline language(const QString val) { this->val = val; }
	inline BlockStream& apply(BlockStream& s) const {
		s.writer()->setAttribute(AttributeLanguage, val);
		return s;
	}
	inline InlineStream& apply(InlineStream& s) const {
		s.writer()->setAttribute(AttributeLanguage, val);
		return s;
	}
private:
	QString val;
};

class heading {
public:
	inline heading(const QString& text) {  this->text = text; }
	inline BlockStream& apply(BlockStream& s) const {
		s.writer()->writeStartElement(TagHeading);
		s.writer()->writeCharacters(text);
		s.writer()->writeEndElement();
		return s;
	}
private:
	QString text;
};

class paragraph {
public:
	inline paragraph(const QString& text) {  this->text = text; }
	inline BlockStream& apply(BlockStream& s) const {
		s.writer()->writeStartElement(TagParagraph);
		s.writer()->writeCharacters(text);
		s.writer()->writeEndElement();
		return s;
	}
private:
	QString text;
};

class abbreviation {
public:
	inline abbreviation(const QString& full) { this->full = full; }
	inline InlineStream& apply(InlineStream& s) const {
		s.writer()->setAttribute(AttributeInlineFullText, QVariant(full));
		s.writer()->writeStartElement(TagTextAbbreviation);
		return s;
	}
private:
	QString full;
};

// TODO improve marks
// class quote {
// public:
// 	inline quote(const QString& mark) { this->mark1 = mark; this->mark2 = mark; }
// 	inline quote(const QString& mark1, const QString& mark2) {
// 		this->mark1 = mark1; this->mark2 = mark2;
// 	}
// 	inline InlineStream& apply(InlineStream& s) const {
// 		s.writer()->setAttribute(AttributeQuoteStartMark, mark1);
// 		s.writer()->setAttribute(AttributeQuoteEndMark, mark2);
// 		s.writer()->writeBeginTag(SpecialTestAbbreviation);
// 		return s;
// 	}
// }


inline BlockStream& BlockStream::operator<<(const Document::heading& h)
{ return h.apply(*this); }
inline BlockStream& BlockStream::operator<<(const Document::paragraph& p)
{ return p.apply(*this); }
inline BlockStream& BlockStream::operator<<(const role& role)
{ return role.apply(*this); }
inline BlockStream& BlockStream::operator<<(const id& id)
{ return id.apply(*this); }
inline BlockStream& BlockStream::operator<<(const classname& name)
{ return name.apply(*this); }
inline BlockStream& BlockStream::operator<<(const language& lang)
{ return lang.apply(*this); }
inline InlineStream& InlineStream::operator<<(const abbreviation& abbr)
{ return abbr.apply(*this); }
inline InlineStream& InlineStream::operator<<(const role& role)
{ return role.apply(*this); }
inline InlineStream& InlineStream::operator<<(const id& id)
{ return id.apply(*this); }
inline InlineStream& InlineStream::operator<<(const classname& name)
{ return name.apply(*this); }
inline InlineStream& InlineStream::operator<<(const language& lang)
{ return lang.apply(*this); }

}

}

#endif
