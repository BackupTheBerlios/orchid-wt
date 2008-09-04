#include "htmlstreams.h"

namespace Orchid {

namespace HTML {

BlockStream& section(BlockStream& s) {
	s.writer()->writeStartElement(HtmlTagSection);
	return s;
}

BlockStream& end(BlockStream& s) {
	s.writer()->writeEndElement();
	return s;
}

InlineStream& code(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextCode);
	return s;
}

InlineStream& definition(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextDefinition);
	return s;
}

InlineStream& emphasis(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextEmphasis);
	return s;
}

InlineStream& keyboard(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextKeyboard);
	return s;
}

InlineStream& quote(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextQuote);
	return s;
}

InlineStream& sample(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextSample);
	return s;
}

InlineStream& span(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextSpan);
	return s;
}

InlineStream& strong(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextStrong);
	return s;
}

InlineStream& subscript(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextSubscript);
	return s;
}

InlineStream& superscript(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextSuperscript);
	return s;
}

InlineStream& variable(InlineStream& s) {
	s.writer()->writeStartElement(HtmlTagTextVariable);
	return s;
}

InlineStream& end(InlineStream& s) {
	s.writer()->writeEndElement();
	return s;
}

}

}
