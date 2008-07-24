#ifndef _STYLE_H_
#define _STYLE_H_

#include "resource.h"

namespace Bamboo {

class Builder;

class Style : public Orchid::Resource::Resource {
public:
	virtual QString content() const;
};

}

#endif 
