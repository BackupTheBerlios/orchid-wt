#include "manuallyshared.h" 

/*!
	\class ManuallySharedDataPointer
	\brief The ManuallySharedDataPointer class provides a pointer to a shared data object.

	\ingroup misc
	
	ManuallySharedDataPointer\<T\> enables to write implicitly or explicitly shared classes where a special detach-algorithmn is needed. It handles reference counting behind the scenes in a thread-safe manner, ensuring that classes that use it can be reentrant.

	Qt provides \c QSharedDataPointer\<T\> and \c QExplicitSharedDatapointer\<T\> to create your own shared classes. This class can be used, where their detach method is insufficient.

	One example is the combination of implicit sharing and deriving the data classes from the base-classes data class.
*/
