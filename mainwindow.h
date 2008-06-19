#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

#include "ui_mainwindow.h"

class TeamListModel;

class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
public:
	MainWindow();
};

#endif
