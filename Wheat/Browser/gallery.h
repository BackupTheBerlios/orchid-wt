#ifndef _ORCHID_GALLERY_H_
#define _ORCHID_GALLERY_H_

#include <leaf/imagecollection.h>

class GalleryPrivate;
class Gallery :
	public Orchid::Resource::IQueryable,
	public Orchid::ImageCollection,
	public Orchid::Resource::IConfigurable
{
public:
	Gallery();
public:
	void query(Orchid::Request*);
	QList<Option> optionList() const;
	QVariant option(const QString& name) const;
	bool setOption(const QString& name, const QVariant& value);
private:
	Q_DECLARE_PRIVATE(Gallery)
};


#endif
