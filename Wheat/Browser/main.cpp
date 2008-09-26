/*
 * This file is part of the Orchid tools applications
 *
 * Copyright (C) 2008 Johannes Bergmeier
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


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
