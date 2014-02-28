//! @file
//! @brief Create an Image Geometry Node.

#ifndef IMAGE_GEOMETRY_HPP
#define IMAGE_GEOMETRY_HPP

#include "osg_headers.hpp"

namespace Viewer
{
  /// @brief Create Image Polygon with Texture coordinate.
  /// @note Image is in the Plan Y=0, and normal is (0,-1,0).
  /// @param[in] width Width of Image.
  /// @param[in] height Height of Image.
  /// @return A Drawable (Geometry) node for OpenSceneGraph.
  osg::ref_ptr<osg::Drawable> createImage(float width, float height);
}
#endif
