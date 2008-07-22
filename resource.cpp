#include "resource.h" 
#include "obuilder.h"

#include <QTextStream>

namespace Orchid {

void RestResource::get(Builder* builder) {
// 	builder->abortMethodNotAllowed();
}

void RestResource::post(Builder* builder) {
// 	builder->abortMethodNotAllowed();
}

void RestResource::put(Builder* builder) {
// 	builder->abortMethodNotAllowed();
}


SimpleTextResource::SimpleTextResource(const QString& text) {
	m_text = text;
}

void SimpleTextResource::get(Builder* builder) {
	QTextStream stream(builder->device());
	stream << m_text;
}

}
