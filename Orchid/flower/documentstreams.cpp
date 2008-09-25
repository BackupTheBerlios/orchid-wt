#include "documentstreams.h"

//
// NOTE due to doxygen only processing one \relates
// the functions related to both BlockStream and
// InlineStream will only be shown in one of them
//

namespace Orchid {

namespace Document {
	
// Functions common to BlockStream and InlineStream

/**
 * \fn RoleManip role(Role val)
 * \relates ::Orchid::Document::BlockStream
 * \relates ::Orchid::Document::InlineStream
 *
 * Same as calling DocumentProcessor::setAttribute(AttributeRole, \a val).
 */

/**
 * \fn IdManip id(const QString &val)
 * \relates ::Orchid::Document::BlockStream
 * \relates ::Orchid::Document::InlineStream
 *
 * Same as calling DocumentProcessor::setAttribute(AttributeId, \a val).
 */

/**
 * \fn ClassnameManip classname(const QString &val)
 * \relates ::Orchid::Document::BlockStream
 * \relates ::Orchid::Document::InlineStream
 *
 * Same as calling DocumentProcessor::setAttribute(AttributeClassaame, \a val).
 */

/**
 * \fn LanguageManip language(const QString &val)
 * \relates ::Orchid::Document::BlockStream
 * \relates ::Orchid::Document::InlineStream
 *
 * Same as calling DocumentProcessor::setAttribute(AttributeLanguage, \a val).
 */


// BlockStream

/**
 * \class BlockStream
 *
 * \brief BlockStream provides an simple to use interface for constructing documents.
 *
 * Using a DocumentProcessor directly for construction of a document
 * requires very much code that is not very readable. BlockStream
 * is part of a simpler API for programmatical construction of documents.
 *
 * It represents the block level of a document.
 *
 * \note Due to a limitation of doxygen some of the functions relating to BlockStream are not listed under the Related Functions-section:
 * \li \c RoleManip role(Role val)
 * \li \c IdManip id(const QString &val)
 * \li \c ClassnameManip classname(const QString &val)
 * \li \c LanguageManip language(const QString &val)
 *
 * Their documentation can be found in the documentation of InlineStream.
 *
 * \sa DocumentProcessor, InlineStream
 */

/**
 * \fn HeadingManip heading(const QString &text)
 * \relates ::Orchid::Document::BlockStream
 *
 * Inserts a heading element with the content \a text to the processor
 * of the stream it was applied to.
 */

/**
 * \fn ParagraphManip paragraph(const QString &text)
 * \relates ::Orchid::Document::BlockStream
 *
 * Inserts a paragraph element with the content \a text to the processor
 * of the stream it was applied to.
 */

/**
 * \relates BlockStream
 *
 * Calls DocumentProcessor::startElement(TagSection) on the processor of \a s and returns \a s.
 */
BlockStream& section(BlockStream& s) {
	s.processor()->startElement(TagSection);
	return s;
}

/**
 * \relates BlockStream
 *
 * Calls DocumentProcessor::endElement() on the processor of \a s and returns \a s.
 */
BlockStream& end(BlockStream& s) {
	s.processor()->endElement();
	return s;
}


/**
 * \class InlineStream
 * 
 * \brief InlineStream provides an simple to use interface for constructing documents.
 *
 * Using a DocumentProcessor directly for construction of a document
 * requires very much code that is not very readable. InlineStream
 * is part of a simpler API for programmatical construction of documents.
 *
 * It represents the inline level of a document which might be a paragraph
 * or some text without special markup.
 *
 * \sa DocumentProcessor, BlockStream
 */

/**
 * \fn DocumentProcessor *InlineStream::processor() const
 *
 * Returns the processor of the stream.
 */

/**
 * \fn AbbreviationManip abbreviation(const QString &full)
 * \relates ::Orchid::Document::InlineStream
 *
 * Starts a abbreviation element with the attribute
 * AttributeInlineFullText set to \a full.
 */


/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextCode) on the processor of \a s and returns \a s.
 */
InlineStream& code(InlineStream& s) {
	s.processor()->startElement(TagTextCode);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextDefinition) on the processor of \a s and returns \a s.
 */
InlineStream& definition(InlineStream& s) {
	s.processor()->startElement(TagTextDefinition);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextEmphasis) on the processor of \a s and returns \a s.
 */
InlineStream& emphasis(InlineStream& s) {
	s.processor()->startElement(TagTextEmphasis);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextKeyboard) on the processor of \a s and returns \a s.
 */
InlineStream& keyboard(InlineStream& s) {
	s.processor()->startElement(TagTextKeyboard);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextQuote) on the processor of \a s and returns \a s.
 */
InlineStream& quote(InlineStream& s) {
	s.processor()->startElement(TagTextQuote);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextSample) on the processor of \a s and returns \a s.
 */
InlineStream& sample(InlineStream& s) {
	s.processor()->startElement(TagTextSample);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextSpan) on the processor of \a s and returns \a s.
 */
InlineStream& span(InlineStream& s) {
	s.processor()->startElement(TagTextSpan);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextStrong) on the processor of \a s and returns \a s.
 */
InlineStream& strong(InlineStream& s) {
	s.processor()->startElement(TagTextStrong);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextSubscript) on the processor of \a s and returns \a s.
 */
InlineStream& subscript(InlineStream& s) {
	s.processor()->startElement(TagTextSubscript);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextSuperscript) on the processor of \a s and returns \a s.
 */
InlineStream& superscript(InlineStream& s) {
	s.processor()->startElement(TagTextSuperscript);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::startElement(TagTextVariable) on the processor of \a s and returns \a s.
 */
InlineStream& variable(InlineStream& s) {
	s.processor()->startElement(TagTextVariable);
	return s;
}

/**
 * \relates InlineStream
 *
 * Calls DocumentProcessor::endElement() on the processor of \a s and returns \a s.
 */
InlineStream& end(InlineStream& s) {
	s.processor()->endElement();
	return s;
}

}

}
