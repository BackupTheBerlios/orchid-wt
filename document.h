#ifndef _BAMBOO_DOCUMENT_H_
#define _BAMBOO_DOCUMENT_H_

#include "fragment.h"

namespace Bamboo {

class Header {
public:
};

class DocumentPrivate;

class Document : public Fragment {
	Q_OBJECT
public:
	Document();
public:
	void build(Builder* builder);
	void setMainFragment(Fragment* fragment);
protected:
	Document(DocumentPrivate& dd);
private:
	Q_DECLARE_PRIVATE(Document);
};

}

#endif
