#ifndef _ORCHID_MODELRESOURCE_P_H_
#define _ORCHID_MODELRESOURCE_P_H_

#include <stem/resourcekeep.h>

class QAbstractItemModel;

namespace Orchid {

class ModelResourcePrivate {
public:
	ModelResourcePrivate(ModelResource* res) : q_ptr(res) {
		model = 0;
	}
protected:
	ModelResource* q_ptr;
private:
	Q_DECLARE_PUBLIC(ModelResource)
	QAbstractItemModel* model;
	Resource::Keep keep;
};

}

#endif 
