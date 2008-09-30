#include "scriptedresource.h"

#include <stem/request.h>
#include <stem/resourcekeep.h>

#include <QtCore/QtDebug>
#include <QtScript/QtScript>

namespace Orchid {
	
class ScriptResource :
	public Resource::Base,
	public Resource::IQueryable,
	public Resource::IDirectory
{
public:
	void query(Orchid::Request*);
	QStringList childs() const;
	Resource::Handle child(const QString& name);
public:
	void setObject(const QScriptValue& object);
protected:
	void *interfaceCast(const char *name);
private:
	Resource::Keep m_keep;
	QScriptValue m_object;
};

class ScriptedResourcePrivate {
	Q_DECLARE_PUBLIC(ScriptedResource)
	friend class ScriptResource;
public:
	ScriptedResourcePrivate(ScriptedResource* res);
protected:
	ScriptedResource* q_ptr;
private:
	static QScriptValue write(QScriptContext *context, QScriptEngine *engine);
private:
	QScriptEngine *engine;
	QString path;
	ScriptResource self;
};


void ScriptResource::query(Orchid::Request* request) {
	if(!m_object.isValid()) {
		// TODO set request status 404
		return;
	}
	
	QScriptValue f(m_object.property("query"));
	if(f.isFunction()) {
		QScriptValue r(m_object.engine()->newQObject(request));
		f.call(m_object, QScriptValueList() << r);
	}
}

QStringList ScriptResource::childs() const {
	if(!m_object.isValid()) return QStringList();
	
	QScriptValue f(m_object.property("childList"));
	if(f.isFunction()) {
		QScriptValue res = f.call(m_object, QScriptValueList());
		if(!res.isArray()) return QStringList();
		
		QStringList list;
		int length = res.property("length").toInt32();
		for(int i = 0; i < length; ++i) {
			list << res.property(i).toString();
		}
		return list;
	}
	return QStringList();
}

Resource::Handle ScriptResource::child(const QString& name) {
	if(!m_object.isValid()) return Resource::Handle();
	
	Resource::Handle handle(m_keep.acquireHandle(name));
	if(!handle.isEmpty()) return handle;

	QScriptValue f(m_object.property("getChild"));
	if(f.isFunction() && !name.contains('/')) {
		QScriptValue nameValue(m_object.engine(), name);

		QScriptValue result = f.call(m_object, QScriptValueList() << nameValue);
		
		if(result.isObject()) {
			ScriptResource* resource = new ScriptResource();
			resource->setObject(result);
			handle.init(resource);
			return handle;
		}
	}
	return Resource::Handle();
}

void ScriptResource::setObject(const QScriptValue &object) {
	m_object = object;
}

void *ScriptResource::interfaceCast(const char *name) {
	ORCHID_PROVIDE_CAST(name, Resource::IQueryable*)
	ORCHID_PROVIDE_CAST(name, Resource::IDirectory*)
	return 0;
}


QScriptValue ScriptedResourcePrivate::write(QScriptContext *context, QScriptEngine *engine) {
	Q_UNUSED(engine);
	QIODevice* device = dynamic_cast<QIODevice*>(context->argument(0).toQObject());
	if(device) {
		if(!(device->isOpen() || device->open(QIODevice::ReadWrite))) {
			qWarning() << "could not open device";
			return QScriptValue();
		}
		QString str(context->argument(1).toString());
		if(!str.isNull()) {
			QTextStream stream(device);
			stream << str;
		}
	}
	return QScriptValue();
}

ScriptedResourcePrivate::ScriptedResourcePrivate(ScriptedResource* res) {
	q_ptr = res;
	engine = 0;
}


ScriptedResource::ScriptedResource() {
	d_ptr = new ScriptedResourcePrivate(this);
}

ScriptedResource::ScriptedResource(const QString &path) {
	d_ptr = new ScriptedResourcePrivate(this);
	loadScript(path);
}

ScriptedResource::~ScriptedResource() {
	delete d_ptr;
}

bool ScriptedResource::loadScript(const QString &url) {
	Q_D(ScriptedResource);
	
	int splitter = url.lastIndexOf('#');
	QString path(url.left(splitter));
	QString type(url.right(url.length()-splitter-1));

	QFile scriptFile(path);
	if(!scriptFile.open(QIODevice::ReadOnly))
		return false;
	QString program(scriptFile.readAll());
	scriptFile.close();

	d->engine = new QScriptEngine();
	d->engine->globalObject().setProperty("write", d->engine->newFunction(ScriptedResourcePrivate::write));
	d->engine->evaluate(program);
	QScriptValue ctor = d->engine->evaluate(type);
	d->self.setObject(ctor.construct());
	
	d->path = path;
	return true;
}

void ScriptedResource::query(Orchid::Request *request) {
	Q_D(ScriptedResource);
	d->self.query(request);
}

QStringList ScriptedResource::childs() const {
	Q_D(const ScriptedResource);
	return d->self.childs();
}

Resource::Handle ScriptedResource::child(const QString& name) {
	Q_D(ScriptedResource);
	return d->self.child(name);
}

QList<Resource::IConfigurable::Option> ScriptedResource::optionList() const {
	QList<Option> optionList;
	optionList << Option("path", qMetaTypeId<QString>());
	return optionList;
}

QVariant ScriptedResource::option(const QString &option) const {
	Q_D(const ScriptedResource);
	QVariant res;
	if(option == "path")
		res = QVariant(d->path);
	return res;
}

bool ScriptedResource::setOption(const QString &option, const QVariant &value) {
	Q_D(ScriptedResource);
	bool result = false;
	if(option == "path") {
		QString newPath = value.toString();
		if(d->path != newPath) {
			result = loadScript(newPath);
		} else {
			result = true;
		}
	}
	return result;
}

}
