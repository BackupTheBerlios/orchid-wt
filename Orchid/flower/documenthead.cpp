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

/**
 * \class DocumentHead
 *
 * \brief DocumentHead provides a representation of the head part of a document.
 *
 * A document head contains the information typically found in the head of a
 * document.
 *
 * DocumentHead isntances are implicitly shared. See the Qt documentation for
 * more infos about implicit sharing.
 *
 * \sa DocumentProcessor
 */

class DocumentHeadPrivate : public QSharedData {
public:
	QString title;
	QVector<QPair<QString,Style*> > styleUrls;
};

/**
 * Constructs a new head.
 */
DocumentHead::DocumentHead() {
	d = new DocumentHeadPrivate;
}

/**
 * Constructs a copy of \a other.
 */
DocumentHead::DocumentHead(const DocumentHead &other) {
	d = other.d;
}

/**
 * Destructs the head.
 */
DocumentHead::~DocumentHead() {
}

/**
 * Assigns \a other to this head and returns a reference to this head.
 */
DocumentHead& DocumentHead::operator=(const DocumentHead &other) {
	d = other.d;
}

/**
 * Returns the title of this head.
 */
QString DocumentHead::title() const {
	return d->title;
}

/**
 * Set the heads title to \a title.
 */
void DocumentHead::setTitle(const QString &title) {
	d->title = title;
}

/**
 * Adds \a style to the list of styles accessible in the document.
 *
 * If \a url is not empty, it can be used by document processors
 * to reference to the style. Otherwise the processors need to inline
 * the style.
 */
void DocumentHead::addStyle(Style *style, const QString &url) {
	d->styleUrls.append(QPair<QString,Style*>(url, style));
}

/**
 * Returns the list of styles as pairs of their urls and the styles.
 */
QVector<QPair<QString,Style*> > DocumentHead::styleUrls() const {
	return d->styleUrls;
}

} 
