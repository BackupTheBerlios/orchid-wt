#include "scriptedresource.h"

#include "scriptedresource.moc"
#include <stem/request.h>
#include <stem/resourcekeep.h>

#include <QtCore/QtDebug>
// #include <QtCore/QExplicitlySharedDataPointer>
#include <QtScript>

namespace Orchid {
	
class ScriptResource :
	public Resource::Base,
	public Resource::IQueryable,
	public Resource::IDirectory,
	public Resource::IDynamic
{
public:
	void query(Orchid::Request*);
	bool provides(InterfaceId id);
	QStringList childs() const;
	Resource::Handle child(const QString& name);
public:
	void setObject(const QScriptValue& object);
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
	QScriptEngine engine;
	ScriptResource self;
};


void ScriptResource::query(Orchid::Request* request) {
	if(!m_object.isValid()) return;
	
	QScriptValue f(m_object.property("query"));
	if(f.isFunction()) {
		QScriptValue r(m_object.engine()->newQObject(request));
		f.call(m_object, QScriptValueList() << r);
	} else {
		qWarning() << "call to disabled resource.query in object" << m_object.toString();
	}
}

bool ScriptResource::provides(InterfaceId id) {
	if(!m_object.isValid()) return false;
	
	if(id == interfaceId<Resource::IQueryable>()) {
		return m_object.property("query").isFunction();
	} else if(id == interfaceId<Resource::IDirectory>()) {
		return m_object.property("childList").isFunction() &&
			m_object.property("getChild").isFunction();
	}
	return false;
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


QScriptValue ScriptedResourcePrivate::write(QScriptContext *context, QScriptEngine *engine) {
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
}

ScriptedResource::ScriptedResource(const QString &script, const QString &type) {
	d_ptr = new ScriptedResourcePrivate(this);
	Q_D(ScriptedResource);
	
	d->engine.globalObject().setProperty("write", d->engine.newFunction(ScriptedResourcePrivate::write));
	
	d->engine.evaluate(script);
	QScriptValue ctor = d->engine.evaluate(type);
	d->self.setObject(ctor.construct());
}

ScriptedResource::~ScriptedResource() {
	delete d_ptr;
}

void ScriptedResource::query(Orchid::Request *request) {
	Q_D(ScriptedResource);
	d->self.query(request);
}

bool ScriptedResource::provides(InterfaceId id) {
	Q_D(ScriptedResource);
	return d->self.provides(id);
}

QStringList ScriptedResource::childs() const {
	Q_D(const ScriptedResource);
	return d->self.childs();
}

Resource::Handle ScriptedResource::child(const QString& name) {
	Q_D(ScriptedResource);
	return d->self.child(name);
}


}
