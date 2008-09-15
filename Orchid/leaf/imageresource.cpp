#include "imageresource.h" 

#include <stem/request.h>
#include <QtCore/QVariant>
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

void ImageResource::setPath(const QString &path) {
	Q_D(ImageResource);
	d_ptr->path = path;
}

void ImageResource::setImage(const QImage &image) {
	Q_D(ImageResource);
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

QList<Resource::IConfigurable::Option> ImageResource::optionList() const {
	QList<Option> optionList;
	optionList << Option("path", qMetaTypeId<QString>());
	optionList << Option("image", qMetaTypeId<QImage>());
	optionList << Option("use-scaling", qMetaTypeId<bool>());
	optionList << Option("width", qMetaTypeId<int>());
	optionList << Option("height", qMetaTypeId<int>());
	return optionList;
}

QVariant ImageResource::option(const QString &option) const {
	Q_D(const ImageResource);
	QVariant res;
	if(option == "path")
		res = QVariant(d->path);
	else if(option == "image")
		res = QVariant(d->image);
	else if(option == "use-scaling")
		res = QVariant(d->useScaling);
	else if(option == "width")
		res = QVariant(d->sizeX);
	else if(option == "height")
		res = QVariant(d->sizeY);
	return res;
}

bool ImageResource::setOption(const QString &option, const QVariant &value) {
	Q_D(ImageResource);
	bool result = false;
	int size;
	if(option == "path") {
		d->path = value.toString();
		d->image = QImage();
		result = true;
	} else if(option == "image") {
		d->path.clear();
		d->image = value.value<QImage>();
		result = true;
	} else if(option == "use-scaling") {
		d->useScaling = value.toBool();
		result = true;
	} else if(option == "width" && (size = value.toInt()) > 0) {
		d->sizeX = value.toInt();
		result = true;
	} else if(option == "height" && (size = value.toInt()) > 0) {
		d->sizeY = value.toInt();
		result = true;
	}
	return result;
}

}
