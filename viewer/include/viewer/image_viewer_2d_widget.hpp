//! @file 2DImageViewerWidget.hpp

#ifndef IMAGE_VIEWER_2D_WIDGET_HPP
#define IMAGE_VIEWER_2D_WIDGET_HPP

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>

#include "image_viewer_2d.hpp"
#include "osg_headers.hpp"
#include "adapter_widget.hpp"

namespace Viewer {

//! @class C2DImageViewerWidget
class ImageViewer2DWidget : public QWidget {
  Q_OBJECT

  ImageViewer2DWidget(const ImageViewer2DWidget &);
  // intentionally undefined
  ImageViewer2DWidget &operator=(const ImageViewer2DWidget &);

 public:
  //! @brief Constructor.
  ImageViewer2DWidget(QWidget *parent = 0);

  //! @brief Destructor.
  virtual ~ImageViewer2DWidget();

 public slots:
  bool slotLoadImage(void);

 private:
  QPushButton *_loadButton;
  Viewer::ImageViewer2D *_imageViewer2D;

  void _setupWidget();
};
}
#endif
