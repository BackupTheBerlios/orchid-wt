#include "htmlstreams.h"

namespace Orchid {

namespace HTML {

BlockStream& section(BlockStream& s) {
	s.writer()->writeBeginTag(HtmlTagSection);
	return s;
}

BlockStream& end(BlockStream& s) {
	s.writer()->writeEndTag();
	return s;
}

InlineStream& code(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextCode);
	return s;
}

InlineStream& definition(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextDefinition);
	return s;
}

InlineStream& emphasis(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextEmphasis);
	return s;
}

InlineStream& keyboard(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextKeyboard);
	return s;
}

InlineStream& quote(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextQuote);
	return s;
}

InlineStream& sample(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextSample);
	return s;
}

InlineStream& span(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextSpan);
	return s;
}

InlineStream& strong(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextStrong);
	return s;
}

InlineStream& subscript(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextSubscript);
	return s;
}

InlineStream& superscript(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextSuperscript);
	return s;
}

InlineStream& variable(InlineStream& s) {
	s.writer()->writeBeginTag(HtmlTagTextVariable);
	return s;
}

InlineStream& end(InlineStream& s) {
	s.writer()->writeEndTag();
	return s;
}

}

}
