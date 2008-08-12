#include "htmlstreams.h"

namespace Bamboo {

namespace HTML {

BlockStream& section(BlockStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialSection);
	return s;
}

BlockStream& end(BlockStream& s) {
	s.writer()->writeEndSpecial();
	return s;
}

InlineStream& code(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextCode);
	return s;
}

InlineStream& definition(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextDefinition);
	return s;
}

InlineStream& emphasis(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextEmphasis);
	return s;
}

InlineStream& keyboard(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextKeyboard);
	return s;
}

InlineStream& quote(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextQuote);
	return s;
}

InlineStream& sample(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextSample);
	return s;
}

InlineStream& span(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextSpan);
	return s;
}

InlineStream& strong(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextStrong);
	return s;
}

InlineStream& subscript(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextSubscript);
	return s;
}

InlineStream& superscript(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextSuperscript);
	return s;
}

InlineStream& variable(InlineStream& s) {
	s.writer()->writeBeginSpecial(HtmlSpecialTextVariable);
	return s;
}

InlineStream& end(InlineStream& s) {
	s.writer()->writeEndSpecial();
	return s;
}

}

}
