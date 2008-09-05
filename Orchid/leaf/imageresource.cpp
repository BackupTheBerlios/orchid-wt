#include "imageresource.h" 

#include <stem/request.h>
#include <QtGui/QImage>

namespace Orchid {

class ImageResourcePrivate {
public:
	ImageResourcePrivate(ImageResource* res);
protected:
	ImageResource* q_ptr;
private:
	QImage image;
	QString path;
	int sizeX, sizeY;
	bool useScaling : 1;
private:
	Q_DECLARE_PUBLIC(ImageResource)
};


ImageResourcePrivate::ImageResourcePrivate(ImageResource* res) {
	q_ptr = res;
	useScaling = false;
}

ImageResource::ImageResource() {
	d_ptr = new ImageResourcePrivate(this);
}

ImageResource::ImageResource(const QString& path) {
	d_ptr = new ImageResourcePrivate(this);
	d_ptr->path = path;
}

ImageResource::ImageResource(const QImage& image) {
	d_ptr = new ImageResourcePrivate(this);
	d_ptr->image = image;
}

void ImageResource::query(Orchid::Request* request) {
	Q_D(ImageResource);
	if(!(GetMethod & request->method())) return;

	if(d->image.isNull()) {
		if(d->path.isNull()) return;
		d->image.load(d->path);
	}

	request->setMimeType("image/jpeg");
	if(!request->open(QIODevice::ReadWrite)) return;

	QImage image(d->image);
	
	if(d->useScaling)
		image = image.scaled(QSize(d->sizeX, d->sizeY), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	
	image.save(request, "jpg");
}

void ImageResource::setScaling(int sizeX, int sizeY) {
	Q_D(ImageResource);
	d->useScaling = true;
	d->sizeX = sizeX;
	d->sizeY = sizeY;
}

}
