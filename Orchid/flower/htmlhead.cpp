#include "htmlhead.h"

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
