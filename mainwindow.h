#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>

#include "resourcekeep.h"

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow, private Ui::MainWindow {
	Q_OBJECT
public:
	MainWindow();
private:
	Orchid::Resource::Handle m_root;
};

#endif
