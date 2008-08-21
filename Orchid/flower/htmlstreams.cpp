#include "htmlstreams.h"

namespace Orchid {

namespace HTML {

BlockStream& section(BlockStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagSection);
	return s;
}

BlockStream& end(BlockStream& s) {
	s.writer()->writeEndSpecial();
	return s;
}

InlineStream& code(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextCode);
	return s;
}

InlineStream& definition(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextDefinition);
	return s;
}

InlineStream& emphasis(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextEmphasis);
	return s;
}

InlineStream& keyboard(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextKeyboard);
	return s;
}

InlineStream& quote(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextQuote);
	return s;
}

InlineStream& sample(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextSample);
	return s;
}

InlineStream& span(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextSpan);
	return s;
}

InlineStream& strong(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextStrong);
	return s;
}

InlineStream& subscript(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextSubscript);
	return s;
}

InlineStream& superscript(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextSuperscript);
	return s;
}

InlineStream& variable(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlTagTextVariable);
	return s;
}

InlineStream& end(InlineStream& s) {
	s.writer()->writeEndSpecial();
	return s;
}

}

}
