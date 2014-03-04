/// @file

#ifndef POINT_CLOUD_GEOMETRY_HPP
#define POINT_CLOUD_GEOMETRY_HPP

#include "type.hpp"

namespace Viewer 
{
  /// @brief Create Point Cloud Polygon with Z Map Texture.
  /// @param[in] p_pfX Float Array for X coordinate. 
  /// @param[in] p_pfY Float Array for Y coordinate.
  /// @param[in] p_pfZ Float Array for Z coordinate.
	/// @param[in] p_uiImageWidth Width of Camera.
	/// @param[in] p_uiImageHeight Height of Camera.
  /// @return A Drawable (Geometry) node for OpenSceneGraph.
  DrawablePtr createPointCloud(
				const float *p_pfX,
				const float *p_pfY,
				const float *p_pfZ,
				unsigned int p_uiImageWidth,
				unsigned int p_uiImageHeight);
	
  /// @brief Create Point Cloud Polygon with Texture.
  /// @param[in] p_pfX Float Array for X coordinate. 
  /// @param[in] p_pfY Float Array for Y coordinate.
  /// @param[in] p_pfZ Float Array for Z coordinate.
  /// @param[in] p_pucRed Unsigned Char Array for Red Channel. 
  /// @param[in] p_pucGreen Unsigned Char Array for Green Channel.
  /// @param[in] p_pucBlue Unsigned Char Array for Blue Channel.
	/// @param[in] p_uiImageWidth Width of Camera.
	/// @param[in] p_uiImageHeight Height of Camera.
  /// @return A Drawable (Geometry) node for OpenSceneGraph.
  DrawablePtr CreateTexturedPointCloud(
				const float *p_pfX,
				const float *p_pfY,
				const float *p_pfZ,
				const unsigned char *p_pucRed,
				const unsigned char *p_pucGreen,
				const unsigned char *p_pucBlue,
				unsigned int p_uiImageWidth,
				unsigned int p_uiImageHeight);
}
#endif
