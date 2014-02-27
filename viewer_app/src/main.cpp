/// @file	ViewerApp/main.cpp
/// @brief Entry point of the program.
/// @author	Corentin Le Molgat 20-Apr-2010

#include <iostream>
#include <QtGui/QApplication>
#include "ViewerGUI/ViewerGUI.hpp"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	if (1 == argc)
	{
		Viewer::ViewerGUI *gui = new Viewer::ViewerGUI();
		// Launch Application
		gui->setMinimumSize(gui->sizeHint());
		return app.exec();
	}
	else
	{
		std::cerr << "usage : ViewerApp" << std::endl;
		return 1;
	}
}
