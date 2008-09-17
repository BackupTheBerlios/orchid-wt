#ifndef _ORCHID_MODELRESOURCE_H_
#define _ORCHID_MODELRESOURCE_H_

#include <stem/resource.h>

class QAbstractItemModel;
class QModelIndex;

namespace Orchid {

class ModelResourcePrivate;
class ModelResource :
	public Resource::Base,
	public Resource::IDirectory,
	public Resource::IConfigurable
{
public:
	ModelResource(QAbstractItemModel* model = 0);
	~ModelResource();
public:
	QAbstractItemModel* model() const;
	void setModel(QAbstractItemModel* model);
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString&);
public:
	virtual void query(Orchid::Request* request, const QModelIndex& index);
	virtual QString name(const QModelIndex& index) const;
	virtual QModelIndex index(const QString& name, const QModelIndex& parent) const;
	virtual QStringList listChilds(const QModelIndex& index) const;
	virtual QList<Option> optionList() const;
	virtual QVariant option(const QString&) const;
	virtual bool setOption(const QString&, const QVariant&);
protected:
	ModelResource(ModelResourcePrivate* d, QAbstractItemModel* model = 0);
private:
	Q_DECLARE_PRIVATE(ModelResource)
};

};

#endif
