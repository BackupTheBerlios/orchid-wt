#ifndef _BAMBOO_FRAGMENT_P_H_
#define _BAMBOO_FRAGMENT_P_H_

#include "fragment.h"

namespace Orchid {

class FragmentPrivate {
public:
	Q_DECLARE_PUBLIC(Fragment);
protected:
	Fragment* q_ptr;
};

}

#endif
