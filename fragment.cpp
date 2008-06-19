#include "fragment.h"
#include "fragment_p.h"

#include "fragment.moc"

namespace Bamboo {

Fragment::Fragment(QObject* parent) : QObject(parent), d_ptr(new FragmentPrivate) {
	d_ptr->q_ptr = this;
}

Fragment::Fragment(FragmentPrivate& dd, QObject* parent) : QObject(parent), d_ptr(&dd) {
	d_ptr->q_ptr = this;
}

Fragment::~Fragment() {
	delete d_ptr;
}

void Fragment::getDependencies(Dependencies& deps) const {
}

}
