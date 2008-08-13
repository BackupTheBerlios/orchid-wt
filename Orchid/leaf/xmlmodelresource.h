#ifndef _ORCHID_XMLMODELRESOURCE_H_
#define _ORCHID_XMLMODELRESOURCE_H_

#include "modelresource.h"

namespace Orchid {

class XmlModelResourcePrivate;
class XmlModelResource : public ModelResource, public Orchid::Resource::IQueryable {
	Q_OBJECT
public:
	XmlModelResource(QAbstractItemModel* model);
	~XmlModelResource();
public:
	void query(Orchid::Request* request);
	void query(Orchid::Request* request, const QModelIndex& index);
private:
	Q_DECLARE_PRIVATE(XmlModelResource)
};

}

#endif
