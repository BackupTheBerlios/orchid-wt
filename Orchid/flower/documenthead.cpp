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


#include "documenthead.h"

#include <QtCore/QVector>
#include <QtCore/QPair>

namespace Orchid {

class DocumentHeadPrivate : public QSharedData {
public:
	QString title;
	QVector<QPair<QString,Style*> > styleUrls;
};

DocumentHead::DocumentHead() {
	d = new DocumentHeadPrivate;
}

DocumentHead::DocumentHead(const DocumentHead &head) {
	d = head.d;
}

DocumentHead::~DocumentHead() {
}

DocumentHead& DocumentHead::operator=(const DocumentHead &other) {
	d = other.d;
}

QString DocumentHead::title() const {
	return d->title;
}

void DocumentHead::setTitle(const QString &title) {
	d->title = title;
}

void DocumentHead::addStyle(Style *style, const QString &url) {
	d->styleUrls.append(QPair<QString,Style*>(url, style));
}

QVector<QPair<QString,Style*> > DocumentHead::styleUrls() const {
	return d->styleUrls;
}

} 
