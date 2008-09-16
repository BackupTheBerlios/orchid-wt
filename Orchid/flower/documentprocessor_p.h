#ifndef _ORCHID_DOCUMENTPROCESSOR_P_H_
#define _ORCHID_DOCUMENTPROCESSOR_P_H_

#include <QHash>

namespace Orchid {

class DocumentProcessorPrivate {
	Q_DECLARE_PUBLIC(DocumentProcessor)
public:
    DocumentProcessorPrivate(DocumentProcessor *processor);
public:
	QHash<const Style*, StyleAttributes> styles;
protected:
	DocumentProcessor* q_ptr;
};


}

#endif
