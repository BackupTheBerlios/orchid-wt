#include "scriptedresource.h"

#include "scriptedresource.moc"
#include <stem/request.h>
#include <stem/resourcekeep.h>

#include <QtCore/QtDebug>
// #include <QtCore/QExplicitlySharedDataPointer>
#include <QtScript>

namespace Orchid {

class ScriptedResourceHandle;
class ScriptedResourceHandleData : public QSharedData {
	friend class ScriptedResourceHandle;
	Resource::Handle handle;
};

/**
 * A wrapper class which makes it possible to ensure no 
 * uninitialized handles are left in the script.
 */
class ScriptedResourceHandle {
public:
	ScriptedResourceHandle();
	ScriptedResourceHandle(const Resource::Handle& handle);
	static ScriptedResourceHandle fromValue(const QScriptValue& value);
public:
	void setHandle(const Resource::Handle& handle);
	QScriptValue createValue(QScriptEngine* engine);
	Resource::Handle handle() const;
	Resource::Handle takeHandle();
private:
	QExplicitlySharedDataPointer<ScriptedResourceHandleData> d;
};

}

Q_DECLARE_METATYPE(Orchid::ScriptedResourceHandle)

namespace Orchid {

ScriptedResourceHandle::ScriptedResourceHandle() {
	d = new ScriptedResourceHandleData;
}

ScriptedResourceHandle::ScriptedResourceHandle(const Resource::Handle& handle) {
	d = new ScriptedResourceHandleData;
	d->handle = handle;
}

ScriptedResourceHandle ScriptedResourceHandle::fromValue(const QScriptValue& value) {
	return value.toVariant().value<ScriptedResourceHandle>();
}

void ScriptedResourceHandle::setHandle(const Resource::Handle& handle) {
	d->handle = handle;
}

QScriptValue ScriptedResourceHandle::createValue(QScriptEngine* engine) {
	QVariant variant;
	variant.setValue(*this);
	QScriptValue val = engine->newVariant(variant);
	return val;
};

Resource::Handle ScriptedResourceHandle::handle() const {
	return d->handle;
}

Resource::Handle ScriptedResourceHandle::takeHandle() {
	Resource::Handle handle = d->handle;
	d->handle = Resource::Handle();
	return handle;
}


// class ScriptedResource

class ScriptedResourcePrivate {
	Q_DECLARE_PUBLIC(ScriptedResource)
public:
	ScriptedResourcePrivate(ScriptedResource* res);
protected:
	ScriptedResource* q_ptr;
private:
	static QScriptValue write(QScriptContext *context, QScriptEngine *engine);
private:
	QScriptEngine engine;
	QScriptValue object;
	Resource::Keep keep;
};

QScriptValue ScriptedResourcePrivate::write(QScriptContext *context, QScriptEngine *engine) {
	QIODevice* device = dynamic_cast<QIODevice*>(context->argument(0).toQObject());
	if(device) {
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
}

ScriptedResource::ScriptedResource(const QString &script, const QString &type) {
	d_ptr = new ScriptedResourcePrivate(this);
	Q_D(ScriptedResource);
	
	d->engine.globalObject().setProperty("write", d->engine.newFunction(ScriptedResourcePrivate::write));
	
	d->engine.evaluate(script);
	QScriptValue ctor = d->engine.evaluate(type);
	d->object = ctor.construct();
	qDebug() << "res.isObject() = " << d->object.isObject();
}

ScriptedResource::~ScriptedResource() {
	delete d_ptr;
}

void ScriptedResource::query(Orchid::Request *request) {
	Q_D(ScriptedResource);
	if(!(request->method() & Orchid::GetMethod)) {
// 		request.setStatus(RequestMethodNotAllowed);
		return;
	}
	if(!request->open(QIODevice::ReadWrite)) return;

	QScriptValue f(d->object.property("query"));
	if(f.isFunction()) {
		f.call(d->object, QScriptValueList() << d->engine.newQObject(request));
	} else {
		qDebug() << "res.query is not a function";
		qDebug() << d->object.toString();
	}
}

bool ScriptedResource::provides(InterfaceId id) {
	Q_D(ScriptedResource);
	if(id == interfaceId<Resource::IQueryable>()) {
		return d->object.property("query").isFunction();
	} else if(id == interfaceId<Resource::IDirectory>()) {
		return d->object.property("childList").isFunction() &&
			d->object.property("getChild").isFunction();
	}
	return false;
}

QStringList ScriptedResource::childs() const {
	Q_D(const ScriptedResource);
	QScriptValue f(d->object.property("childList"));
	if(f.isFunction()) {
		QScriptValue res = f.call(d->object, QScriptValueList());
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

Resource::Handle ScriptedResource::child(const QString& name) {
	Q_D(ScriptedResource);
	QScriptValue f(d->object.property("getChild"));
	if(f.isFunction()) {
		Resource::Handle handle(d->keep.getHandle(name));
		ScriptedResourceHandle scriptedHandle(handle);
		QScriptValue handleValue(scriptedHandle.createValue(&d->engine));
		QScriptValue nameValue(&d->engine, name);

		QScriptValue res = f.call(d->object, QScriptValueList() << handleValue << nameValue);

		handle = ScriptedResourceHandle::fromValue(res).handle();

		// ensure that there is no copy of the handle left in the script
		scriptedHandle.takeHandle();
		
		if(!handle.isEmpty())
			return handle;
	}
	return Resource::Handle();
}


}
