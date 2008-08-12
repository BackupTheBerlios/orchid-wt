#ifndef _BAMBOO_DOCUMENT_H_
#define _BAMBOO_DOCUMENT_H_

#include "fragment.h"

namespace Orchid {

class Style;

class DocumentPrivate;

class Document : public Fragment {
	Q_OBJECT
public:
	Document();
public:
	void build(HtmlStreamWriter* writer);
	void setMainFragment(Fragment* fragment);
public:
	void addGlobalStyle(Style*);
	void addGlobalStyle(Style*, const QString& url);
protected:
	Document(DocumentPrivate& dd);
private:
	Q_DECLARE_PRIVATE(Document);
};

}

#endif
