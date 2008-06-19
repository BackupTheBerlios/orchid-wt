#ifndef _DEPENDENCY_H_
#define _DEPENDENCY_H_

#include <QSet>

namespace Bamboo {

class Builder;

}

enum DependencyType {
	UnknownDependency = 0,
	DependencyExternalScript = 1,
	DependencyExternalStyle = 2,
};

class Dependency {
public:
// 	Dependency();
// 	Dependency(const Dependency& dep);
// 	~Dependency();
public:
	uint hash() const;
	void writeToHead(Bamboo::Builder* builder) const;
public:
	bool operator==(const Dependency&) const;
public:
	static Dependency externalStylesheet(const QString& source, const QString& media = QString());
private:
	QString m_source;
	QString m_media;
	DependencyType m_type;
};

// }

uint qHash(const Dependency&);

class Dependencies : public QSet<Dependency> {
};


#endif 
