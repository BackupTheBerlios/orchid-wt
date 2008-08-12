#ifndef _STYLE_H_
#define _STYLE_H_

#include <QtCore/QString>

namespace Bamboo {

class Style {
public:
	virtual QString content() const;
};

class Css30Style : public Style {
public:
	QString classdef();
};

}

#endif 
