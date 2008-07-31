#ifndef _ORCHID_HTTPSERVICE_PRIVATE_H_
#define _ORCHID_HTTPSERVICE_PRIVATE_H_

#include <QtCore/QString>

class HttpService;
class QAbstractSocket;

namespace Orchid {

class HttpServiceProcess : public QObject {
	Q_OBJECT
	public:
		HttpServiceProcess(HttpService* service, QAbstractSocket* socket);
		~HttpServiceProcess();
	public slots:
		void read();
	private:
		HttpService* m_service;
		QString m_requestStr;
		QAbstractSocket* m_socket;
};

}

#endif
