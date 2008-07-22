#ifndef _ORCHID_CLIENTINFO_
#define _ORCHID_CLIENTINFO_

#include "manuallyshared.h"
#include "global.h"

class QHttpRequestHeader;

namespace Orchid {


class ClientInfoData;

class ClientInfo {
public:
	ClientInfo();
	ClientInfo(const QHttpRequestHeader& header);
	ClientInfo(const ClientInfo& client);
	~ClientInfo();
protected:
	ManuallySharedDataPointer<ClientInfoData> s_ptr;
private:
	O_DECLARE_SHARED(ClientInfo);
};

class BrowserInfoData;

class BrowserInfo : public ClientInfo {
public:
	BrowserInfo();
	BrowserInfo(const ClientInfo& client);
	BrowserInfo(const BrowserInfo& browser);
protected:
	O_DECLARE_SHARED(BrowserInfo);
};

}

#endif
