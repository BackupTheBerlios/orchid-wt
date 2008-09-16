#ifndef _ORCHID_IMAGERESOURCE_H_
#define _ORCHID_IMAGERESOURCE_H_

#include <QtCore/QtGlobal>
#include <stem/resource.h>

class QString;
class QImage;

namespace Orchid {

class ImageResourcePrivate;
class ImageResource :
	public Resource::IResource,
	public Resource::IQueryable,
	public Resource::IConfigurable
{
public:
	ImageResource();
	ImageResource(const QString& path);
	ImageResource(const QImage& image);
public:
	QString path() const;
	void setPath(const QString& path);
	QImage image() const;
	void setImage(const QImage& image);
	void query(Orchid::Request*);
	void setScaling(int sizeX, int sizeY);
	QList<Option> optionList() const;
	QVariant option(const QString&) const;
	bool setOption(const QString&, const QVariant&);
protected:
	ImageResourcePrivate* d_ptr;
private:
	Q_DECLARE_PRIVATE(ImageResource)
};


}

#endif
