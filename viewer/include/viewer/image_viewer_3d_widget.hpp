/// @file 3DImageViewerWidget.hpp

#ifndef _3D_IMAGE_VIEWER_WIDGET_HPP
#define _3D_IMAGE_VIEWER_WIDGET_HPP

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include "image_viewer_3d.hpp"

namespace Viewer {
//! @class C3DImageViewerWidget
class ImageViewer3DWidget : public QWidget {
  Q_OBJECT

  ImageViewer3DWidget(const ImageViewer3DWidget &);
  // intentionally undefined
  ImageViewer3DWidget &operator=(const ImageViewer3DWidget &);

 public:
  //! @brief Constructor.
  ImageViewer3DWidget(QWidget *parent = 0);

  //! @brief Destructor.
  virtual ~ImageViewer3DWidget();

 public slots:
  bool slotLoadImage(void);

 private:
  QPushButton *_loadButton;
  Viewer::ImageViewer3D *_imageViewer3D;

  void _setupWidget();
};
}  // DataGrabberGUI

#endif  // end of include guard: 3D_IMAGE_VIEWER_HPP
