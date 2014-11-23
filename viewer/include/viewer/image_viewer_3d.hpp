//! @file

#ifndef IMAGE_VIEWER_3D_HPP
#define IMAGE_VIEWER_3D_HPP

#include "adapter_widget.hpp"
#include "type.hpp"

namespace Viewer {
//! @class C3DImageViewer
class ImageViewer3D : public ViewerQT {
  Q_OBJECT

  ImageViewer3D(const ImageViewer3D&);
  // intentionally undefined
  ImageViewer3D& operator=(const ImageViewer3D&);

 public:
  //! @brief Constructor.
  ImageViewer3D(QWidget* parent = 0);

  //! @brief Destructor.
  virtual ~ImageViewer3D();

 public slots:
  bool slotSet3DImage(const DrawablePtr& image);

  bool slotSet3DImages(const DrawablePtrArray& images);

 private:
  bool addImage(const DrawablePtr& image);

  void _setupWidget();

  osg::ref_ptr<osg::Group> _rootNode;
};
}
#endif
