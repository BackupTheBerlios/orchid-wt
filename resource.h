#ifndef _ORCHID_RESOURCE_H_ 
#define _ORCHID_RESOURCE_H_

#include <QString>

namespace Orchid {

class Resource {
public:
	QString url() const;
private:
	QString m_url;
};

};

#endif
