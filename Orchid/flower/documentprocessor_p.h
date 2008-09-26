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


#ifndef _ORCHID_DOCUMENTPROCESSOR_P_H_
#define _ORCHID_DOCUMENTPROCESSOR_P_H_

#include <QHash>

namespace Orchid {

class DocumentProcessorPrivate {
	Q_DECLARE_PUBLIC(DocumentProcessor)
public:
    DocumentProcessorPrivate(DocumentProcessor *processor);
public:
	QHash<const Style*, StyleAttributes> styles;
protected:
	DocumentProcessor* q_ptr;
};


}

#endif
