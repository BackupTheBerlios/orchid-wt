#include "modelresource.h"
#include "modelresource_p.h"

#include <QtCore/QAbstractItemModel>
#include <stem/request.h>
#include <QtCore/QTextStream>

namespace Orchid {

class ModelItemResource : public Resource::Base, public Resource::IDirectory, public Resource::IQueryable  {
	friend class ModelResource;
public:
	ModelItemResource(ModelResource* root, const QModelIndex& index);
public:
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString&);
	void query(Orchid::Request*);
private:
	ModelResource* root;
	QModelIndex index;
	Orchid::Resource::Keep keep;
};

ModelItemResource::ModelItemResource(ModelResource* root, const QModelIndex& index) {
	this->index = index;
	this->root = root;
}

QStringList ModelItemResource::childs() const {
	return root->listChilds(index);
}

Resource::Handle ModelItemResource::child(const QString &name) {
	Orchid::Resource::Handle handle = keep.acquireHandle(name);
	if(handle.isEmpty()) {
		handle.init(new ModelItemResource(root, root->index(name, index)));
	}
	
	return handle;
}

void ModelItemResource::query(Orchid::Request* request) {
	root->query(request, index);
}

	
ModelResource::ModelResource(QAbstractItemModel* model)
	: Base(new ModelResourcePrivate(this))
{
	Q_D(ModelResource);
	d->model = model;
}

ModelResource::ModelResource(ModelResourcePrivate* dptr, QAbstractItemModel* model)
	: Base(dptr)
{
	Q_D(ModelResource);
	d->model = model;
}

ModelResource::~ModelResource() {
}

QAbstractItemModel* ModelResource::model() const {
	Q_D(const ModelResource);
	return d->model;
}

void ModelResource::setModel(QAbstractItemModel* model) {
	Q_D(ModelResource);
	if(d->model == model) return;
	if(d->model) {
		d->keep.resetAll();
	}
	d->model = model;
}


QStringList ModelResource::childs() const {
	return listChilds(QModelIndex());
}

Resource::Handle ModelResource::child(const QString& name) {
	Q_D(ModelResource);
	
	Orchid::Resource::Handle handle = d->keep.acquireHandle(name);
	if(handle.isEmpty()) {
		handle.init(new ModelItemResource(this, index(name, QModelIndex())));
	}
	
	return handle;
}

void ModelResource::query(Orchid::Request* request, const QModelIndex& index) {
	if(!index.isValid()) return;
	if(!request->open(QIODevice::ReadWrite)) return;
	QTextStream stream(request);
	stream << "<h1>" << index.data().toString() << "</h1>\n"
		<< "Parent: \"" << index.parent().data().toString() << "\"<br/>\nRow: " << index.row() << "<nr/>\nColumn: " << index.column() << "<br/>\n"
		<< "Second-column: " << index.model()->index(index.row(), 1, index.parent()).data().toString();
}

QString ModelResource::name(const QModelIndex& index) const {
	return QString("%1-"/*"%2-"*/"%3").arg(index.row())./*arg(index.column()).*/arg(index.data().toString());
}

QModelIndex ModelResource::index(const QString& name, const QModelIndex& parent) const {
	Q_D(const ModelResource);
	if(!d->model) return QModelIndex();

	QStringList parts = name.split('-');
	int row = parts[0].toInt();
// 	int col = parts[1].toInt();
	return d->model->index(row, 0, parent);
}

QStringList ModelResource::listChilds(const QModelIndex& parent) const {
	Q_D(const ModelResource);
	if(!d->model) return QStringList();

	QStringList list;
	int rows = d->model->rowCount(parent);
// 	int cols = d->model->columnCount(parent);
	for(int i = 0; i < rows; ++i) {
// 		for(int j = 0; j < cols; ++j)
			list.append(name(d->model->index(i, 0, parent)));
	}
	return list;
}

QList<Resource::IConfigurable::Option> ModelResource::optionList() const {
	return QList<Option>() << Option("model", qMetaTypeId<QObject*>());
}

QVariant ModelResource::option(const QString &option) const {
	Q_D(const ModelResource);
	if(option == "model")
		return QVariant(d->model);
	return QVariant();
}

bool ModelResource::setOption(const QString &option, const QVariant &value) {
	if(option == "model") {
		if(value.isNull()) {
			setModel(0);
			return true;
		} else {
			QObject *object = value.value<QObject*>();
			QAbstractItemModel *model = qobject_cast<QAbstractItemModel*>(object);
			if(model) {
				setModel(model);
				return true;
			}
		}
	}
	return false;
}

}
