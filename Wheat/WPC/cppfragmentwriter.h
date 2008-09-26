/*
 * This file is part of the Orchid tools applications
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _ORCHID_CPPFRAGMENTWRITER_H_
#define _ORCHID_CPPFRAGMENTWRITER_H_

#include <QtCore/QtGlobal>

class QTextStream;

namespace Orchid {

class DomFragment;

class CppFragmentWriterPrivate;
class CppFragmentWriter {
public:
	CppFragmentWriter(QTextStream* stream);
	~CppFragmentWriter();
public:
	void write(DomFragment *fragment);
protected:
	CppFragmentWriterPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(CppFragmentWriter)
};

class CppHeaderWriterPrivate;
class CppHeaderWriter {
public:
	CppHeaderWriter(QTextStream* stream);
	~CppHeaderWriter();
public:
	bool addFragment(DomFragment *fragment);
	void write();
protected:
	CppHeaderWriterPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(CppHeaderWriter)
};

}

#endif
