#ifndef _ORCHID_MANUALLY_SHARED_
#define _ORCHID_MANUALLY_SHARED_

template <class T> class ManuallySharedDataPointer {
public:
	inline T &operator*() { return *d; }
	inline const T &operator*() const { return *d; }
	inline T *operator->() { return d; }
	inline const T *operator->() const { return d; }
	inline operator T *() { return d; }
	inline operator const T *() const { return d; }
	inline T *data() { return d; }
	inline const T *data() const { return d; }
	inline const T *constData() const { return d; }
	
	inline bool operator==(const ManuallySharedDataPointer<T> &other) const { return d == other.d; }
	inline bool operator!=(const ManuallySharedDataPointer<T> &other) const { return d != other.d; }
	
	inline ManuallySharedDataPointer() { d = 0; }
	inline ~ManuallySharedDataPointer() { if (d && !d->ref.deref()) delete d; }
	
	explicit ManuallySharedDataPointer(T *data);
	inline ManuallySharedDataPointer(const ManuallySharedDataPointer<T> &o) : d(o.d) { if(d) d->ref.ref(); }
	inline ManuallySharedDataPointer<T> & operator=(const ManuallySharedDataPointer<T> &o) {
		if(o.d != d) {
			if(o.d)
				o.d->ref.ref();
			if(d && !d->ref.deref())
				delete d;
			d = o.d;
		}
		return *this;
	}
	inline ManuallySharedDataPointer &operator=(T *o) {
		if(o != d) {
			if(o)
				o->ref.ref();
			if(d && !d->ref.deref())
				delete d;
			d = o;
		}
		return *this;
	}
	
	inline bool operator!() const { return !d; }
private:
	T *d;
};

#endif
