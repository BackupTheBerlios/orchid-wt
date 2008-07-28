#include "resource.h" 
#include "obuilder.h"
#include "resourcekeep.h"

#include <QTextStream>

namespace Orchid {

namespace Resource {

Handle Resource::locateUrl(const Handle& handle, const QUrl& url) {
	const Resource* res = handle.resource();
	if(url.isRelative()) {
		QStringList path = url.path().split('/');
		if(path.isEmpty()) return Handle();
		
		Handle handle;
		
		while(res) {
			const IRedirecting* redir = dynamic_cast<const IRedirecting*>(res);
			if(redir) {
				return redir->locate(url);
			}

			const IDirectory* dir = dynamic_cast<const IDirectory*>(res);
			if(dir) {
				handle = dir->child(path.takeFirst());
			}
			
			if(path.isEmpty())
				return handle;
			
			res = handle.resource();
		}
		
	}
	return Handle();
}

}

void RestResource::methodGet(Builder* builder) {
// 	builder->abortMethodNotAllowed();
}

void RestResource::methodPost(Builder* builder) {
// 	builder->abortMethodNotAllowed();
}

void RestResource::methodPut(Builder* builder) {
// 	builder->abortMethodNotAllowed();
}


SimpleTextResource::SimpleTextResource(const QString& text) {
	m_text = text;
}

void SimpleTextResource::methodGet(Builder* builder) {
	QTextStream stream(builder->device());
	stream << m_text;
}

}
