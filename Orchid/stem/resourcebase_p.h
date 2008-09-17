#ifndef _ORCHID_RESOURCEBASE_P_H_
#define _ORCHID_RESOURCEBASE_P_H_

#include "resourcekeep.h"

namespace Orchid {

namespace Resource {

class BasePrivate {
	Q_DECLARE_PUBLIC(Base)
public:
	BasePrivate(Base *base);
protected:
	Base *q_ptr;
private:
	Keep keep;
};

}

}

#endif
