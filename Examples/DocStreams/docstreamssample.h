#ifndef _ORCHID_DOCSTREAMSSAMPLE_H_
#define _ORCHID_DOCSTREAMSSAMPLE_H_

#include <stem/resource.h>
#include <flower/documenthead.h>

namespace Orchid {
class Fragment;
class Style;
}

class DocStreamsSample : public Orchid::Resource::Base, public Orchid::Resource::IQueryable {
public:
	DocStreamsSample();
	~DocStreamsSample();
public:
	void query(Orchid::Request* request);
private:
	Orchid::DocumentHead m_head;
	Orchid::Fragment* m_body;
	Orchid::Style* m_style;
};

#endif
