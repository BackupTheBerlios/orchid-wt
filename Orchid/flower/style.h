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


#ifndef _STYLE_H_
#define _STYLE_H_

#include "globals.h"

#include <QtCore/QString>

namespace Orchid {

class ORCHID_FLOWER_EXPORT Style {
public:
	virtual QString content() const;
};

class ORCHID_FLOWER_EXPORT Css30Style : public Style {
public:
	QString classdef();
};

}

#endif 
