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


#include "request.h"
#include "request_p.h" 

#include "resource.h"
#include "resourcekeep.h"

#include <QtCore/QUrl>
#include <QtCore/QtDebug>

namespace Orchid {

Request::Request() {
    d_ptr = new RequestPrivate(this);
}

Request::Request(RequestPrivate* dd) {
	d_ptr = dd;
}

Request::~Request() {
	delete d_ptr;
}

void Request::setRoot(const Resource::Handle& root) {
	Q_D(Request);
	d->locations.setRoot(root);
}

Resource::Handle Request::addLocation(const Resource::Location& location) {
	Q_D(Request);
	return d->locations.addLocation(location);
}

Resource::Location Request::resolve(const Resource::Location& location) const {
	Q_D(const Request);
	return d->locations.resolve(location);
}

Resource::Location Request::location() const {
	Q_D(const Request);
	return d->location;
}

Resource::Handle Request::resource() const {
	Q_D(const Request);
	return d->resource;
}

void Request::setLocation(const Resource::Location& location) {
	Q_D(Request);
	d->resource = addLocation(location);
	d->location = location;
}

bool Request::query() {
	Q_D(Request);
	Resource::Base* resource = d->resource.resource();
	Resource::IQueryable* res = Resource::cast<Resource::IQueryable*>(resource);
	if(!res) {
		qWarning() << "Request not found:" << d->location.path();
// 		setStatus(RequestNotFound);
		return false;
	}

	qDebug() << "Process request:" << d->location.path();
	res->query(this);
	return true;
}


RequestMethod Request::method() const { return UnknownMethod; }

void Request::setMimeType(const QString& type) {
}

QString Request::url(const Resource::Location& location) const {
	return QString();
}



SimpleRequest::SimpleRequest()
	: Request(new SimpleRequestPrivate(this))
{
}

QIODevice* SimpleRequest::readDevice() const {
	Q_D(const SimpleRequest);
	return d->readDevice;
}

QIODevice* SimpleRequest::writeDevice() const {
	Q_D(const SimpleRequest);
	return d->writeDevice;
}

void SimpleRequest::setDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

void SimpleRequest::setReadDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

void SimpleRequest::setWriteDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

qint64 SimpleRequest::readData(char* data, qint64 size) {
	Q_D(SimpleRequest);
	return d->readDevice->read(data, size);
}

qint64 SimpleRequest::writeData(const char* data, qint64 size) {
	Q_D(SimpleRequest);
	return d->writeDevice->write(data, size);
}

RequestMethod SimpleRequest::method() const {
	return GetMethod;
}

bool SimpleRequest::open(QIODevice::OpenMode mode) {
	Q_D(SimpleRequest);
	if(d->readDevice && mode.testFlag(QIODevice::ReadOnly) &&
		d->readDevice->openMode() == QIODevice::NotOpen)
	{
		if(!d->readDevice->open(mode & ~QIODevice::ReadOnly))
			return false;
	}
	if(d->writeDevice && mode.testFlag(QIODevice::WriteOnly) &&
		d->writeDevice->openMode() == QIODevice::NotOpen)
	{
		if(!d->writeDevice->open(mode & ~(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate)))
			return false;
	}
	
	return QIODevice::open(mode);
}


}
