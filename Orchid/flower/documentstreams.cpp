#include "documentstreams.h"

namespace Orchid {

namespace Document {

BlockStream& section(BlockStream& s) {
	s.writer()->startElement(TagSection);
	return s;
}

BlockStream& end(BlockStream& s) {
	s.writer()->endElement();
	return s;
}

InlineStream& code(InlineStream& s) {
	s.writer()->startElement(TagTextCode);
	return s;
}

InlineStream& definition(InlineStream& s) {
	s.writer()->startElement(TagTextDefinition);
	return s;
}

InlineStream& emphasis(InlineStream& s) {
	s.writer()->startElement(TagTextEmphasis);
	return s;
}

InlineStream& keyboard(InlineStream& s) {
	s.writer()->startElement(TagTextKeyboard);
	return s;
}

InlineStream& quote(InlineStream& s) {
	s.writer()->startElement(TagTextQuote);
	return s;
}

InlineStream& sample(InlineStream& s) {
	s.writer()->startElement(TagTextSample);
	return s;
}

InlineStream& span(InlineStream& s) {
	s.writer()->startElement(TagTextSpan);
	return s;
}

InlineStream& strong(InlineStream& s) {
	s.writer()->startElement(TagTextStrong);
	return s;
}

InlineStream& subscript(InlineStream& s) {
	s.writer()->startElement(TagTextSubscript);
	return s;
}

InlineStream& superscript(InlineStream& s) {
	s.writer()->startElement(TagTextSuperscript);
	return s;
}

InlineStream& variable(InlineStream& s) {
	s.writer()->startElement(TagTextVariable);
	return s;
}

InlineStream& end(InlineStream& s) {
	s.writer()->endElement();
	return s;
}

}

}
