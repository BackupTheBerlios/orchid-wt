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

#include <QTextStream>
#include <QDir>

#include <stem/extensionmanager.h>

#include "mainwindow.h"

bool loadPlugin(const QString &path) {
	return Orchid::ExtensionManager::loadExtension(path);
}

bool loadPluginDir(const QString &path) {
	QDir pluginsDir(path);
	if(!pluginsDir.exists()) {
		return false;
	}
    foreach(QString fileName, pluginsDir.entryList(QDir::Files)) {
		QString path = pluginsDir.absoluteFilePath(fileName);
		Orchid::ExtensionManager::loadExtension(path);
    }
	return true;
}

void showHelp() {
	QTextStream help(stdout, QIODevice::WriteOnly);
	help << "Orchid version " << ORCHID_VERSION_STR << endl;
}

int main(int argc, char** argv) {
    Q_INIT_RESOURCE(browser);
    QApplication app(argc, argv);

	int state = 0;
	QStringList args = app.arguments();
	QString appname = args[0];
	QStringList::const_iterator it;
	for(it = ++args.constBegin(); it != args.constEnd(); ++it) {
		const QString &arg = *it;
		switch(state) {
			case 0:
				if(arg == "-h" || arg == "--help") {
					state = -2;
				} else if(arg == "-p" || arg == "--add-plugin") {
					state = 1;
				} else if(arg == "-P" || arg == "--add-plugin-dir") {
					state = 2;
				} else {
					QTextStream out(stderr, QIODevice::WriteOnly);
					out << "Unrecognized option " << arg << endl;
					state = -1;
				}
				break;
			case 1: // Awaits plugin
				if(!loadPlugin(arg)) {
					QTextStream out(stderr, QIODevice::WriteOnly);
					out << "Could not load plugin " << arg << endl;
				}
				state = 0;
				break;
			case 2: // Awaits plugin directory
				if(!loadPluginDir(arg)) {
					QTextStream out(stderr, QIODevice::WriteOnly);
					out << "Could not load plugins in directory " << arg << endl;
				}
				state = 0;
				break;
			default:
				Q_ASSERT(false && "This should never be the case");
				break;
		}
		if(state < 0) break;
	}
	if(state != 0) {
		showHelp();
		if(state == -2) { // called with --help
			return 0;
		} else {
			return state;
		}
	}

	MainWindow win;
	win.show();

    return app.exec();
}
