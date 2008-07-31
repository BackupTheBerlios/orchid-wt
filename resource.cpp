#include "resource.h" 
#include "request.h"
#include "resourcekeep.h"

#include <QTextStream>

#include <QtDebug>

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

SimpleTextResource::SimpleTextResource(const QString& text) {
	m_text = text;
}

void SimpleTextResource::query(Request* request) {
	qDebug() << "query";
	if(!request->open(QIODevice::ReadWrite)) return;
	QTextStream stream(request);
	stream << m_text;
}

}
