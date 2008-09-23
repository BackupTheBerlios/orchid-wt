#include "documentprocessor.h" 
#include "documentprocessor_p.h"

#include "style.h"
#include "styleattributes.h"

namespace Orchid {

DocumentProcessorPrivate::DocumentProcessorPrivate(DocumentProcessor* processor)
	: q_ptr(processor)
{
}

DocumentProcessor::DocumentProcessor()
	: d_ptr(new DocumentProcessorPrivate(this))
{
}

DocumentProcessor::DocumentProcessor(DocumentProcessorPrivate* dd)
	: d_ptr(dd)
{
}

DocumentProcessor::~DocumentProcessor() {
	delete d_ptr;
}

StyleAttributes DocumentProcessor::attributes(const Style* style) {
	Q_D(DocumentProcessor);
	return d->styles.value(style);
}

void DocumentProcessor::regStyle(const Style* style, const QString& prefix) {
	Q_D(DocumentProcessor);
	d->styles.insert(style, StyleAttributes(prefix));
}

QString DocumentProcessor::defaultRoleName(Document::Role role) {
	switch(role) {
		case Document::RoleDefinition:
			return "definition";
	}
	Q_ASSERT(false);
}

}
