#ifndef _ORCHID_DOCUMENTSTREAMS_H_
#define _ORCHID_DOCUMENTSTREAMS_H_

#include <QtCore/QString>
#include "documentprocessor.h"
#include <QtCore/QVariant>

namespace Orchid {

namespace Document {

class RoleManip;
class IdManip;
class ClassnameManip;
class LanguageManip;

class HeadingManip;
class ParagraphManip;

class AbbreviationManip;

class InlineStream;
class BlockStream {
public:
	inline BlockStream(DocumentProcessor* processor);
public:
	inline DocumentProcessor* processor() const;
	inline BlockStream& operator<<(BlockStream&(*)(BlockStream&));
	inline InlineStream heading();
	inline InlineStream paragraph();
	inline InlineStream text();
	inline BlockStream& operator<<(const HeadingManip& obj);
	inline BlockStream& operator<<(const ParagraphManip& obj);
	inline BlockStream& operator<<(const RoleManip& role);
	inline BlockStream& operator<<(const IdManip& id);
	inline BlockStream& operator<<(const ClassnameManip& name);
	inline BlockStream& operator<<(const LanguageManip& lang);
private:
	DocumentProcessor* m_processor;
};

inline BlockStream::BlockStream(DocumentProcessor* processor)
	: m_processor(processor) {}

inline DocumentProcessor* BlockStream::processor() const { return m_processor; }
inline BlockStream& BlockStream::operator<<(BlockStream&(*fp)(BlockStream&))
{ return fp(*this); }


// class TextManip;
class InlineStream {
	friend class BlockStream;
public:
	inline InlineStream(DocumentProcessor* processor);
	inline InlineStream(InlineStream& other);
	inline ~InlineStream();
public:
	inline DocumentProcessor* processor() const;
	inline InlineStream& operator<<(InlineStream&(*)(InlineStream&));
	inline InlineStream& operator<<(QChar c);
	inline InlineStream& operator<<(const QString& str);
// 	inline InlineStream& operator<<(const TextManip&);
	inline InlineStream& operator<<(const RoleManip& role);
	inline InlineStream& operator<<(const IdManip& id);
	inline InlineStream& operator<<(const AbbreviationManip& abbr);
	inline InlineStream& operator<<(const ClassnameManip& name);
	inline InlineStream& operator<<(const LanguageManip& lang);
private:
	inline InlineStream(BlockStream& block);
	Q_DISABLE_COPY(InlineStream);
// 	InlineStream& operator=(const InlineStream& other); 
private:
	DocumentProcessor* m_processor;
	bool m_inBlock;
};

inline InlineStream::InlineStream(DocumentProcessor* processor)
	: m_processor(processor), m_inBlock(false) { }
inline InlineStream::InlineStream(InlineStream& other)
	: m_processor(other.processor()), m_inBlock(other.m_inBlock)
{ other.m_inBlock = false; }
inline InlineStream::InlineStream(BlockStream& block)
	: m_processor(block.processor()), m_inBlock(true) { }
inline InlineStream::~InlineStream() 
{ if(m_inBlock) m_processor->endElement(); }
	
inline DocumentProcessor* InlineStream::processor() const { return m_processor; }
inline InlineStream& InlineStream::operator<<(QChar c)
{ m_processor->insertCharacters(c); return *this; }
inline InlineStream& InlineStream::operator<<(const QString& str)
{ m_processor->insertCharacters(str); return *this; }
inline InlineStream& InlineStream::operator<<(InlineStream&(*fp)(InlineStream&))
{ return fp(*this); }

inline InlineStream BlockStream::heading()
{ m_processor->startElement(TagHeading); return InlineStream(*this); }
inline InlineStream BlockStream::paragraph()
{ m_processor->startElement(TagParagraph); return InlineStream(*this); }
inline InlineStream BlockStream::text()
{ return InlineStream(m_processor); }


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

class RoleManip {
public:
	inline RoleManip(Role val) { this->val = val; }
	inline BlockStream& apply(BlockStream& s) const {
		s.processor()->setAttribute(AttributeRole, val);
		return s;
	}
	inline InlineStream& apply(InlineStream& s) const {
		s.processor()->setAttribute(AttributeRole, val);
		return s;
	}
private:
	Role val;
};
inline RoleManip role(Role val) { return RoleManip(val); }

class IdManip {
public:
	inline IdManip(const QString &val) { this->val = val; }
	inline BlockStream& apply(BlockStream& s) const {
		s.processor()->setAttribute(AttributeId, val);
		return s;
	}
	inline InlineStream& apply(InlineStream& s) const {
		s.processor()->setAttribute(AttributeId, val);
		return s;
	}
private:
	QString val;
};
inline IdManip id(const QString &val) { return IdManip(val); }

class ClassnameManip {
public:
	inline ClassnameManip(const QString &val) { this->val = val; }
	inline BlockStream& apply(BlockStream& s) const {
		s.processor()->setAttribute(AttributeClassname, val);
		return s;
	}
	inline InlineStream& apply(InlineStream& s) const {
		s.processor()->setAttribute(AttributeClassname, val);
		return s;
	}
private:
	QString val;
};
inline ClassnameManip classname(const QString &val) { return ClassnameManip(val); }

class LanguageManip {
public:
	inline LanguageManip(const QString &val) { this->val = val; }
	inline BlockStream& apply(BlockStream& s) const {
		s.processor()->setAttribute(AttributeLanguage, val);
		return s;
	}
	inline InlineStream& apply(InlineStream& s) const {
		s.processor()->setAttribute(AttributeLanguage, val);
		return s;
	}
private:
	QString val;
};
inline LanguageManip language(const QString &val) { return LanguageManip(val); }

class HeadingManip {
public:
	inline HeadingManip(const QString& text) {  this->text = text; }
	inline BlockStream& apply(BlockStream& s) const {
		s.processor()->startElement(TagHeading);
		s.processor()->insertCharacters(text);
		s.processor()->endElement();
		return s;
	}
private:
	QString text;
};
inline HeadingManip heading(const QString &text) { return HeadingManip(text); }

class ParagraphManip {
public:
	inline ParagraphManip(const QString& text) {  this->text = text; }
	inline BlockStream& apply(BlockStream& s) const {
		s.processor()->startElement(TagParagraph);
		s.processor()->insertCharacters(text);
		s.processor()->endElement();
		return s;
	}
private:
	QString text;
};
inline ParagraphManip paragraph(const QString &text) { return ParagraphManip(text); }

class AbbreviationManip {
public:
	inline AbbreviationManip(const QString& full) { this->full = full; }
	inline InlineStream& apply(InlineStream& s) const {
		s.processor()->setAttribute(AttributeInlineFullText, QVariant(full));
		s.processor()->startElement(TagTextAbbreviation);
		return s;
	}
private:
	QString full;
};
inline AbbreviationManip abbreviation(const QString &full) { return AbbreviationManip(full); }

// TODO improve marks
// class quote {
// public:
// 	inline quote(const QString& mark) { this->mark1 = mark; this->mark2 = mark; }
// 	inline quote(const QString& mark1, const QString& mark2) {
// 		this->mark1 = mark1; this->mark2 = mark2;
// 	}
// 	inline InlineStream& apply(InlineStream& s) const {
// 		s.processor()->setAttribute(AttributeQuoteStartMark, mark1);
// 		s.processor()->setAttribute(AttributeQuoteEndMark, mark2);
// 		s.processor()->writeBeginTag(SpecialTestAbbreviation);
// 		return s;
// 	}
// }


inline BlockStream& BlockStream::operator<<(const HeadingManip& h)
{ return h.apply(*this); }
inline BlockStream& BlockStream::operator<<(const ParagraphManip& p)
{ return p.apply(*this); }
inline BlockStream& BlockStream::operator<<(const RoleManip& role)
{ return role.apply(*this); }
inline BlockStream& BlockStream::operator<<(const IdManip& id)
{ return id.apply(*this); }
inline BlockStream& BlockStream::operator<<(const ClassnameManip& name)
{ return name.apply(*this); }
inline BlockStream& BlockStream::operator<<(const LanguageManip& lang)
{ return lang.apply(*this); }
inline InlineStream& InlineStream::operator<<(const AbbreviationManip& abbr)
{ return abbr.apply(*this); }
inline InlineStream& InlineStream::operator<<(const RoleManip& role)
{ return role.apply(*this); }
inline InlineStream& InlineStream::operator<<(const IdManip& id)
{ return id.apply(*this); }
inline InlineStream& InlineStream::operator<<(const ClassnameManip& name)
{ return name.apply(*this); }
inline InlineStream& InlineStream::operator<<(const LanguageManip& lang)
{ return lang.apply(*this); }

}

}

#endif
