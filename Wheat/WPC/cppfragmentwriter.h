#ifndef _ORCHID_CPPFRAGMENTWRITER_H_
#define _ORCHID_CPPFRAGMENTWRITER_H_

#include <QtCore/QtGlobal>

class QTextStream;

namespace Orchid {

class DomFragment;

class CppFragmentWriterPrivate;
class CppFragmentWriter {
public:
	CppFragmentWriter(QTextStream* stream);
	~CppFragmentWriter();
public:
	void write(DomFragment *fragment);
protected:
	CppFragmentWriterPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(CppFragmentWriter)
};

class CppHeaderWriterPrivate;
class CppHeaderWriter {
public:
	CppHeaderWriter(QTextStream* stream);
	~CppHeaderWriter();
public:
	bool addFragment(DomFragment *fragment);
	void write();
protected:
	CppHeaderWriterPrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(CppHeaderWriter)
};

}

#endif
