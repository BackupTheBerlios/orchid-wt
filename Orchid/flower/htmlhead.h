#ifndef _ORCHID_HTMLHEAD_H_
#define _ORCHID_HTMLHEAD_H_

#include <QtCore/QSharedData>
#include <QtCore/QString>

template<typename t> class QVector;
template<typename t1, typename t2> class QPair;

namespace Orchid {

class Style;

class HtmlHeadPrivate;
class HtmlHead {
public:
	HtmlHead();
	HtmlHead(const HtmlHead &head);
	~HtmlHead();
public:
	QString title() const;
	void setTitle(const QString &title);
	void addStyle(Style *style, const QString& url = QString());
	QVector<QPair<QString,Style*> > styleUrls() const;
	HtmlHead& operator=(const HtmlHead &other);
private:
	QSharedDataPointer<HtmlHeadPrivate> d;
};

}

#endif
