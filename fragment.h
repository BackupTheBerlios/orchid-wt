#ifndef _BAMBOO_FRAGMENT_H_
#define _BAMBOO_FRAGMENT_H_

#include <QObject>

namespace Bamboo {

class Builder;
class FragmentPrivate;

class Fragment : public QObject {
	Q_OBJECT
public:
	explicit Fragment(QObject* parent = 0);
	virtual ~Fragment();
public:
	virtual void build(Builder* builder) = 0;
protected:
    Fragment(FragmentPrivate &dd, QObject* parent = 0);
	FragmentPrivate*const d_ptr;
private:
	Q_DECLARE_PRIVATE(Fragment);
};

}

#endif
