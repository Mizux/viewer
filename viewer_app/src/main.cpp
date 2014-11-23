/// @file	ViewerApp/main.cpp
/// @brief Entry point of the program.
/// @author	Corentin Le Molgat 20-Apr-2010

#include <iostream>
#include <QtGui/QApplication>
#include <viewer/image_viewer_2d_widget.hpp>
#include <viewer/image_viewer_3d_widget.hpp>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  if (1 == argc) {
    Viewer::ImageViewer2DWidget *gui2D = new Viewer::ImageViewer2DWidget();
    Viewer::ImageViewer3DWidget *gui3D = new Viewer::ImageViewer3DWidget();
    // Launch Application
    gui2D->setMinimumSize(gui2D->sizeHint());
    gui2D->show();
    gui3D->setMinimumSize(gui3D->sizeHint());
    gui3D->show();
    return app.exec();
  } else {
    std::cerr << "usage : ViewerApp" << std::endl;
    return 1;
  }
}
