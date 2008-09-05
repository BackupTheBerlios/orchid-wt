#ifndef _ORCHID_GALLERY_H_
#define _ORCHID_GALLERY_H_

#include <leaf/imagecollection.h>

class GalleryPrivate;
class Gallery : public Orchid::Resource::IQueryable, public Orchid::ImageCollection {
public:
	Gallery();
public:
	void query(Orchid::Request*);
// 	QStringList childs() const;
// 	Orchid::Resource::Handle child(const QString&);
protected:
	GalleryPrivate *d_ptr;
private:
	Q_DECLARE_PRIVATE(Gallery)
// 	QString m_file;
// 	Orchid::Resource::Keep m_keep;
};


#endif
