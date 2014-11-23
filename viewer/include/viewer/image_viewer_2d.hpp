/// @file

#ifndef _2D_IMAGE_VIEWER_HPP
#define _2D_IMAGE_VIEWER_HPP

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>

#include "osg_headers.hpp"
#include "adapter_widget.hpp"

namespace Viewer {
//! @class C2DImageViewer
class ImageViewer2D : public ViewerQT {
  Q_OBJECT

  ImageViewer2D(const ImageViewer2D&);
  // intentionally undefined
  ImageViewer2D& operator=(const ImageViewer2D&);

 public:
  //! @brief Constructor.
  ImageViewer2D(QWidget* parent = 0);

  //! @brief Destructor.
  virtual ~ImageViewer2D();

 public slots:
  bool slotSet2DImage(const QImage& image);

  bool slotSet2DImages(const QVector<QImage>& images);

 private:
  bool _addImage(const QImage& image);

  void _setupWidget();

  osg::ref_ptr<osg::Group> _rootNode;
};
}  // DataGrabberGUI

#endif  // end of include guard: 2D_IMAGE_VIEWER_HPP
