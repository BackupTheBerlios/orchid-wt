#include "dependency.h" 

#include "builder.h"

#include <QXmlStreamWriter>

// namespace Bamboo {

uint Dependency::hash() const {
// 	return ::qHash(::qHash(m_source) + ::qHash(m_media) + m_type);
	return 1;
}

bool Dependency::operator==(const Dependency& dep) const {
	return (m_type == dep.m_type) && (m_source == dep.m_source) && (m_media == dep.m_media);
}

void Dependency::writeToHead(Bamboo::Builder* builder) const {
	QXmlStreamWriter* xml = builder->xml();
	switch(m_type) {
		case UnknownDependency: break;
		case DependencyExternalScript:
			break;
		case DependencyExternalStyle:
			xml->writeEmptyElement("link");
			xml->writeAttribute("rel", "stylesheet");
			xml->writeAttribute("type", "text/css");
			if(!m_media.isEmpty())
				xml->writeAttribute("media", m_media);
			xml->writeAttribute("href", m_source);
			break;
	}
}

Dependency Dependency::externalStylesheet(const QString& source, const QString& media) {
	Dependency dep;
	dep.m_type = DependencyExternalStyle;
	dep.m_source = source;
	dep.m_media = media;
	return dep;
}

// }

uint qHash(const Dependency& dep) {
	return dep.hash();
}
