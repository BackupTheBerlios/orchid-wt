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


#ifndef _ORCHID_DOCUMENTHEAD_H_
#define _ORCHID_DOCUMENTHEAD_H_

#include <QtCore/QSharedData>
#include <QtCore/QString>

template<typename t> class QVector;
template<typename t1, typename t2> class QPair;

namespace Orchid {

class Style;

class DocumentHeadPrivate;
class DocumentHead {
public:
	DocumentHead();
	DocumentHead(const DocumentHead &other);
	~DocumentHead();
public:
	QString title() const;
	void setTitle(const QString &title);
	void addStyle(Style *style, const QString& url = QString());
	QVector<QPair<QString,Style*> > styleUrls() const;
	DocumentHead& operator=(const DocumentHead &other);
private:
	QSharedDataPointer<DocumentHeadPrivate> d;
};

}

#endif
