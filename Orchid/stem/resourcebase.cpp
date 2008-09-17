#include "resourcebase.h"
#include "resourcebase_p.h"

namespace Orchid {

namespace Resource {

BasePrivate::BasePrivate(Base *base) {
	q_ptr = base;
}

Base::Base() {
	d_ptr = new BasePrivate(this);
}

Base::Base(BasePrivate *d) {
	d_ptr = d;
}

Base::~Base() {
	delete d_ptr;
}

Keep *Base::keep() {
	Q_D(Base);
	return &d->keep;
}

}

}
