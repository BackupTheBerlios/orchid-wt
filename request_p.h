#ifndef _ORCHID_BUILDER_P_H_
#define _ORCHID_BUILDER_P_H_

namespace Orchid {

class RequestPrivate {
	Q_DECLARE_PUBLIC(Request)
public:
    RequestPrivate(Request *request)
        : q_ptr(request)
    { }
private:
    Request* q_ptr;
	QIODevice* device;
};

}


#endif
