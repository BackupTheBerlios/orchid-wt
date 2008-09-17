#ifndef _ORCHID_RESOURCEBASE_H_
#define _ORCHID_RESOURCEBASE_H_

#include <QtCore/QtGlobal>

namespace Orchid {

namespace Resource {
class Keep;
class KeepPrivate;

class BasePrivate;
class Base {
	Q_DECLARE_PRIVATE(Base)
	friend class KeepPrivate;
public:
	Base();
	virtual ~Base();
protected:
	Keep *keep();
	Base(BasePrivate *d);
protected:
	BasePrivate *d_ptr;
};
}

}

#endif
