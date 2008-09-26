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

/**
 * \enum RequestStatus
 *
 * \note RequestStatus currently not used.
 */

/**
 * \enum RequestMethod
 *
 * \brief RequestMethod contains flags for the method used
 * for a request.
 */

/**
 * \var RequestMethod UnknownMethod
 *
 * The method is not known
 */

/**
 * \var RequestMethod GetMethod
 *
 * Identifies a get request. This might be a HTTP GET request.
 */

/**
 * \var RequestMethod HttpGetMethod
 *
 * Identifies a HTTP GET request.
 */

/**
 * \var RequestMethod HttpPostMethod
 *
 * Identifies a HTTP POST request. This method is currently
 * not used.
 */

/**
 * \var RequestMethod HttpHeadMethod
 *
 * Identifies a HTTP HEAD request. This method is currently
 * not used.
 */

/**
 * \var RequestMethod HttpPutMethod
 *
 * Identifies a HTTP PUT request. This method is currently
 * not used.
 */

/**
 * \var RequestMethod HttpDeleteMethod
 *
 * Identifies a HTTP DELETE request. This method is currently
 * not used.
 */

/**
 * \var RequestMethod HttpOptionsMethod
 *
 * Identifies a HTTP OPTIONS request. This method is currently
 * not used.
 */

/**
 * \class Request
 *
 * \brief Request provides a interface for getting data from
 * requests and sending responses.
 *
 * A request provides some information about the request it
 * represents and an I/O device for reading the request and writing
 * a response.
 *
 * \sa Resource::Base, Resource::Location, Service
 */

/**
 * Constructs a new request.
 */
Request::Request() {
    d_ptr = new RequestPrivate(this);
}

/**
 * \internal
 */
Request::Request(RequestPrivate* dd) {
	d_ptr = dd;
}

/**
 * Destructs the request.
 */
Request::~Request() {
	delete d_ptr;
}

/**
 * Sets the root of the request to \a root.
 */
void Request::setRoot(const Resource::Handle& root) {
	Q_D(Request);
	d->locations.setRoot(root);
}

/**
 * Adds \a location to the requests location lookup and returns
 * the handle of \a location.
 *
 * \sa LocationLookup
 */
Resource::Handle Request::addLocation(const Resource::Location& location) {
	Q_D(Request);
	return d->locations.addLocation(location);
}

/**
 * Returns \a location relative to the root of the request.
 * This can be handy to create the URL for a resource.
 */
Resource::Location Request::resolve(const Resource::Location& location) const {
	Q_D(const Request);
	return d->locations.resolve(location);
}

/**
 * Returns the location of the requested resource.
 */
Resource::Location Request::location() const {
	Q_D(const Request);
	return d->location;
}

/**
 * Returns the requested resource.
 */
Resource::Handle Request::resource() const {
	Q_D(const Request);
	return d->resource;
}

/**
 * Sets the resource at \a location as the requested resource.
 */
void Request::setLocation(const Resource::Location& location) {
	Q_D(Request);
	d->resource = addLocation(location);
	d->location = location;
}

/**
 * Calls IQueryable::query() on the requested resource and returns
 * true if the resource provides this interface or returns false
 * otherwise.
 */
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

/**
 * Returns the method of the request. You should reimplement this
 * method in derived classes.
 */
RequestMethod Request::method() const { return UnknownMethod; }

/**
 * Sets the MIME-type of the response to \a type. This implementation
 * does nothing. Reimplement it for requests that can use the
 * MIME-type information.
 */
void Request::setMimeType(const QString& type) {
}

/**
 * Returns the url of the resource at \a location. This implementation
 * returns an empty string. Reimplement it for requests that support
 * request by urls.
 */
QString Request::url(const Resource::Location& location) const {
	return QString();
}

/**
 * \class SimpleRequest
 *
 * \brief SimpleRequest provides an request with two custom
 * I/O devices for reading and writing.
 *
 * \warning I'm not absolutely sure if this implementation works
 * well with the underlying QIODevice.
 */

/**
 * Constructs a new simple request.
 */
SimpleRequest::SimpleRequest()
	: Request(new SimpleRequestPrivate(this))
{
}

/**
 * Returns the device for reading.
 *
 * \sa writeDevice(), setReadDevice(), setDevice()
 */
QIODevice* SimpleRequest::readDevice() const {
	Q_D(const SimpleRequest);
	return d->readDevice;
}

/**
 * Returns the device for writing.
 *
 * \sa readDevice(), setWriteDevice(), setDevice()
 */
QIODevice* SimpleRequest::writeDevice() const {
	Q_D(const SimpleRequest);
	return d->writeDevice;
}

/**
 * Sets both the reading and the writing device to \a device.
 *
 * \sa setReadDevice(), setWriteDevice()
 */
void SimpleRequest::setDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

/**
 * Sets the device for reading to \a device
 *
 * \sa readDevice(), setDevice(), setWriteDevice()
 */
void SimpleRequest::setReadDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

/**
 * Sets the device for writing to \a device
 *
 * \sa writeDevice(), setDevice(), setReadDevice()
 */
void SimpleRequest::setWriteDevice(QIODevice* device) {
	Q_D(SimpleRequest);
	d->readDevice = device;
	d->writeDevice = device;
}

/**
 * The implementation for actual reading. See the QIODevice
 * documentation for more information.
 */
qint64 SimpleRequest::readData(char* data, qint64 size) {
	Q_D(SimpleRequest);
	return d->readDevice->read(data, size);
}

/**
 * The implementation for actual writing. See the QIODevice
 * documentation for more information.
 */
qint64 SimpleRequest::writeData(const char* data, qint64 size) {
	Q_D(SimpleRequest);
	return d->writeDevice->write(data, size);
}

/**
 * Return GetMethod as the typical SimpleRequest will be used
 * to act on get requests. Reimplement for requests with other
 * methods.
 */
RequestMethod SimpleRequest::method() const {
	return GetMethod;
}

/**
 * Opens the read and the write device accordingly to \a mode
 * and calls QIODevice::open(). Returns true on success or
 * false otherwise.
 */
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
