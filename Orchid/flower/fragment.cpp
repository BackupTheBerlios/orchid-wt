#include "fragment.h"
#include "fragment_p.h"

#include "fragment.moc"

namespace Orchid {

/**
 * \class Fragment
 *
 * \brief Fragment provides an abstract interface for ...
 *
 * TODO finish documentation
 */

/**
 * \fn void Fragment::build(DocumentProcessor *processor) = 0
 *
 * Adds the content of the fragment to \a processor.
 */

/**
 * Constructs a new fragment.
 */
Fragment::Fragment(QObject* parent) : QObject(parent), d_ptr(new FragmentPrivate) {
	d_ptr->q_ptr = this;
}

/**
 * \internal
 */
Fragment::Fragment(FragmentPrivate& dd, QObject* parent) : QObject(parent), d_ptr(&dd) {
	d_ptr->q_ptr = this;
}

/**
 * Destructs the fragment.
 */
Fragment::~Fragment() {
	delete d_ptr;
}

}
