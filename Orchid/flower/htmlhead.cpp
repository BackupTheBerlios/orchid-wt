#include "htmlhead.h"

#include <QtCore/QVector>
#include <QtCore/QPair>

namespace Orchid {

class HtmlHeadPrivate : public QSharedData {
public:
	QString title;
	QVector<QPair<QString,Style*> > styleUrls;
};

HtmlHead::HtmlHead() {
	d = new HtmlHeadPrivate;
}

HtmlHead::HtmlHead(const HtmlHead &head) {
	d = head.d;
}

HtmlHead::~HtmlHead() {
}

HtmlHead& HtmlHead::operator=(const HtmlHead &other) {
	d = other.d;
}

QString HtmlHead::title() const {
	return d->title;
}

void HtmlHead::setTitle(const QString &title) {
	d->title = title;
}

void HtmlHead::addStyle(Style *style, const QString &url) {
	d->styleUrls.append(QPair<QString,Style*>(url, style));
}

QVector<QPair<QString,Style*> > HtmlHead::styleUrls() const {
	return d->styleUrls;
}

} 
