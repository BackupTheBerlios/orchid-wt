/*
 * This file is part of the Orchid libraries
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#ifndef _ORCHID_HTTPSERVICE_PRIVATE_H_
#define _ORCHID_HTTPSERVICE_PRIVATE_H_

#include "service_p.h"

#include <QtCore/QString>
#include <QtNetwork/QTcpServer>

class HttpService;
class QAbstractSocket;

namespace Orchid {

class ORCHID_ROOT_EXPORT HttpServiceProcess : public QObject {
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

class ORCHID_ROOT_EXPORT HttpServicePrivate : public ServicePrivate {
public:
	HttpServicePrivate(HttpService* service) : ServicePrivate(service) {}
public:
	int port;
	QTcpServer server;
private:
	Q_DECLARE_PUBLIC(HttpService);
};

}

#endif
