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


#include "fragment.h"
#include "fragment_p.h"

#include "fragment.moc"

namespace Orchid {

/**
 * \class Fragment
 *
 * \brief Fragment provides an abstract interface for ...
 *
 * TODO finish documentation
 */

/**
 * \fn void Fragment::build(DocumentProcessor *processor) = 0
 *
 * Adds the content of the fragment to \a processor.
 */

/**
 * Constructs a new fragment.
 */
Fragment::Fragment(QObject* parent) : QObject(parent), d_ptr(new FragmentPrivate) {
	d_ptr->q_ptr = this;
}

/**
 * \internal
 */
Fragment::Fragment(FragmentPrivate& dd, QObject* parent) : QObject(parent), d_ptr(&dd) {
	d_ptr->q_ptr = this;
}

/**
 * Destructs the fragment.
 */
Fragment::~Fragment() {
	delete d_ptr;
}

}
