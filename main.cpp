#include <QApplication>

#include "mainwindow.h"
#include "httpservice.h"
#include "resource.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);

	Orchid::HttpService service(8000);
	Orchid::SimpleTextResource myRes("Hello World");

	service.setRoot(&myRes);

	MainWindow win;
	win.show();

    return app.exec();
}
