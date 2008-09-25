#ifndef _ORCHID_RESOURCEBASE_H_
#define _ORCHID_RESOURCEBASE_H_

#include <QtCore/QString>

// NOTE i hope this check works on other compilers too

#define ORCHID_RESOURCE(NAME) \
public: \
	template<class T> inline void checkForResourceMacro(T*) const { const T* p = this; } \
	inline static QString staticTypeName() { return NAME; } \
	virtual QString typeName() const { return NAME; } \
private:

namespace Orchid {

namespace Resource {
class Keep;
class KeepPrivate;

class BasePrivate;
class Base {
	ORCHID_RESOURCE("Resource")
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
