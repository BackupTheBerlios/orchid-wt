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


#ifndef _BAMBOO_FRAGMENT_H_
#define _BAMBOO_FRAGMENT_H_

#include <QObject>

// TODO makes it really much sense to derive Fragment from QObject ?

namespace Orchid {

class DocumentProcessor;
class FragmentPrivate;

class Fragment : public QObject {
	Q_OBJECT
public:
	explicit Fragment(QObject* parent = 0);
	virtual ~Fragment();
public:
	virtual void build(DocumentProcessor* processor) = 0;
protected:
    Fragment(FragmentPrivate &dd, QObject* parent = 0);
	FragmentPrivate*const d_ptr;
private:
	Q_DECLARE_PRIVATE(Fragment);
};

}

#endif
