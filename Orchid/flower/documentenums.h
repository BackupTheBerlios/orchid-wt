#ifndef _ORCHID_DOCUMENTENUMS_H_
#define _ORCHID_DOCUMENTENUMS_H_

namespace Orchid {

namespace Document {

enum Tag {
	TagUnknown = 0,
	TagBlock = 1,
	TagSection,
	TagHeading,
	TagParagraph,
	TagTextAbbreviation,
	TagTextCode,
	TagTextDefinition,
	TagTextEmphasis,
	TagTextKeyboard,
	TagTextQuote,
	TagTextSample,
	TagTextSpan,
	TagTextStrong,
	TagTextSubscript,
	TagTextSuperscript,
	TagTextVariable,
	TagCount // Only used for dimensioning lookups
};

enum Attribute {
	AttributeInlineFullText = 1,
	AttributeRole,
	AttributeId,
	AttributeClassname,
	AttributeLanguage,
	AttributeListSize
};

enum Role {
	RoleDefinition = 1,
};

}

}

#endif
