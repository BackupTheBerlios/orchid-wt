#include <QApplication>

#include "mainwindow.h"
// #include "httpservice.h"
// #include "resource.h"

int main(int argc, char** argv) {
    Q_INIT_RESOURCE(browser);
    QApplication app(argc, argv);

	MainWindow win;
	win.show();

    return app.exec();
}
