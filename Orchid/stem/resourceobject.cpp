#include "resourceobject.h"

#include "resourceobject.moc"

#include "resourcekeep.h"

namespace Orchid {

namespace Resource {

/**
 * \class Object
 *
 * \brief Resource::Object provides the base for standard object.
 */

/**
 * Returns the list child names. Reimplement this if you want to
 * provide childs.
 *
 * \sa childResource()
 */
QStringList Object::childs() const {
	return QStringList();
}

/**
 * Returns the child \a name. You shouldn't need to reimplement this
 * method. Reimplement childResource() instead.
 *
 * \sa childResource()
 */
Handle Object::child(const QString &name) {
	Handle handle = keep()->acquireHandle(name);
	
	// Create the resource if it doesn't exist yet
	if(handle.isEmpty())
		childResource(handle);
	
	// Make sure not to return uninitialized handles
	if(handle.isEmpty())
		handle = Handle();
	return handle;
}

void *Object::interfaceCast(const char *name) {
	// NOTE: qt_metacast is not part of the documented API
	// but as it is accessed inline in the Q_DECLARE_INTERFACE macro
	// it is ensured that this method stays the whole Qt 4 liftime
	return qt_metacast(name);
}

/**
 * This method gets called if the requested \a handle was not initialized
 * yet. Reimplementations should initialize \a handle if its request
 * was valid.
 *
 * \sa childs()
 */
void Object::childResource(Handle &handle) {
}

}

}
