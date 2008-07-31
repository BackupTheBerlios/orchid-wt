#ifndef _ORCHID_REQUEST_PRIVATE_H_
#define _ORCHID_REQUEST_PRIVATE_H_

namespace Orchid {

class RequestPrivate {
	Q_DECLARE_PUBLIC(Request)
public:
    RequestPrivate(Request *request)
        : q_ptr(request)
    { }
protected:
    Request* q_ptr;
};

class SimpleRequestPrivate : public RequestPrivate {
	Q_DECLARE_PUBLIC(SimpleRequest)
public:
	SimpleRequestPrivate(SimpleRequest *request)
		: RequestPrivate(request)
	{ }
public:
	QIODevice* readDevice;
	QIODevice* writeDevice;
};

}


#endif
