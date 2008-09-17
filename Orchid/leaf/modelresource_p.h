#ifndef _ORCHID_MODELRESOURCE_P_H_
#define _ORCHID_MODELRESOURCE_P_H_

#include <stem/resourcebase_p.h>
#include <stem/resourcekeep.h>

class QAbstractItemModel;

namespace Orchid {

class ModelResourcePrivate : public Resource::BasePrivate {
public:
	ModelResourcePrivate(ModelResource* res)
		: BasePrivate(res)
	{
		model = 0;
	}
private:
	Q_DECLARE_PUBLIC(ModelResource)
	QAbstractItemModel* model;
	Resource::Keep keep;
};

}

#endif 
