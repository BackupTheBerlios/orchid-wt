#ifndef _ORCHID_GALLERY_H_
#define _ORCHID_GALLERY_H_

#include <stem/resource.h>

class GalleryPrivate;
class Gallery :
	public Orchid::Resource::IResource,
	public Orchid::Resource::IQueryable,
	public Orchid::Resource::IDirectory,
	public Orchid::Resource::IConfigurable
{
public:
	Gallery();
	~Gallery();
public:
	void insertFile(const QString &name, const QString &file);
	
	void query(Orchid::Request*);
	QStringList childs() const;
	Orchid::Resource::Handle child(const QString &name);
	QList<Option> optionList() const;
	QVariant option(const QString& name) const;
	bool setOption(const QString& name, const QVariant& value);
protected:
	GalleryPrivate *d_ptr;
private:
	Q_DECLARE_PRIVATE(Gallery)
};


#endif
