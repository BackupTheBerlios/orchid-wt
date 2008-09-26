/*
 * This file is part of the Orchid libraries
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


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
