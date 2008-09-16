#ifndef _ORCHID_DOCSTREAMSSAMPLE_H_
#define _ORCHID_DOCSTREAMSSAMPLE_H_

#include <stem/resource.h>
#include <flower/htmlhead.h>

namespace Orchid {
class Fragment;
class Style;
}

class DocStreamsSample : public Orchid::Resource::IResource, public Orchid::Resource::IQueryable {
public:
	DocStreamsSample();
	~DocStreamsSample();
public:
	void query(Orchid::Request* request);
private:
	Orchid::HtmlHead m_head;
	Orchid::Fragment* m_body;
	Orchid::Style* m_style;
};

#endif
