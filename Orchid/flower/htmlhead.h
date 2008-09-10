#ifndef _ORCHID_HTMLHEAD_H_
#define _ORCHID_HTMLHEAD_H_

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
	DocumentHead(const DocumentHead &head);
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
