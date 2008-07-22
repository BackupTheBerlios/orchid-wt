#ifndef _ORCHID_CORE_GLOBAL_H_
#define _ORCHID_CORE_GLOBAL_H_

#define O_DECLARE_SHARED(Class) \
protected: \
	virtual void detach_helper(); \
private: \
    void detach(); \
    inline Class##Data* d_func() { detach(); return reinterpret_cast<Class##Data *>(s_ptr.data()); } \
    inline const Class##Data* d_func() const { return reinterpret_cast<const Class##Data *>(s_ptr.constData()); } \
    friend class Class##Data;
	
#define O_SHARED_IMPLEMENTATION(Class) \
void Class::detach_helper() { \
	s_ptr = new Class##Data(*s_ptr); \
} \
inline void Class::detach() { if (s_ptr && s_ptr->ref != 1) detach_helper(); }
    
 
#endif
