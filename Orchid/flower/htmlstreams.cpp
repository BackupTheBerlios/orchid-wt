#include "htmlstreams.h"

namespace Orchid {

namespace Document {

BlockStream& section(BlockStream& s) {
	s.writer()->writeStartElement(TagSection);
	return s;
}

BlockStream& end(BlockStream& s) {
	s.writer()->writeEndElement();
	return s;
}

InlineStream& code(InlineStream& s) {
	s.writer()->writeStartElement(TagTextCode);
	return s;
}

InlineStream& definition(InlineStream& s) {
	s.writer()->writeStartElement(TagTextDefinition);
	return s;
}

InlineStream& emphasis(InlineStream& s) {
	s.writer()->writeStartElement(TagTextEmphasis);
	return s;
}

InlineStream& keyboard(InlineStream& s) {
	s.writer()->writeStartElement(TagTextKeyboard);
	return s;
}

InlineStream& quote(InlineStream& s) {
	s.writer()->writeStartElement(TagTextQuote);
	return s;
}

InlineStream& sample(InlineStream& s) {
	s.writer()->writeStartElement(TagTextSample);
	return s;
}

InlineStream& span(InlineStream& s) {
	s.writer()->writeStartElement(TagTextSpan);
	return s;
}

InlineStream& strong(InlineStream& s) {
	s.writer()->writeStartElement(TagTextStrong);
	return s;
}

InlineStream& subscript(InlineStream& s) {
	s.writer()->writeStartElement(TagTextSubscript);
	return s;
}

InlineStream& superscript(InlineStream& s) {
	s.writer()->writeStartElement(TagTextSuperscript);
	return s;
}

InlineStream& variable(InlineStream& s) {
	s.writer()->writeStartElement(TagTextVariable);
	return s;
}

InlineStream& end(InlineStream& s) {
	s.writer()->writeEndElement();
	return s;
}

}

}
