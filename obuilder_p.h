#ifndef _ORCHID_BUILDER_P_H_
#define _ORCHID_BUILDER_P_H_

namespace Orchid {

class BuilderPrivate {
	Q_DECLARE_PUBLIC(Builder)
public:
    BuilderPrivate(Builder *builder)
        : q_ptr(builder)
    { }
private:
    Builder* q_ptr;
	QIODevice* device;
};

}


#endif
