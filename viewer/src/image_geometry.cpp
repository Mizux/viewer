//! @file
//! @brief Create an Image Geometry Node.

#include <viewer/image_geometry.hpp>

#include <osg/Geometry>

namespace Viewer {

DrawablePtr createDrawable(float width, float height) {
  // Setup the Geometry.
  osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;

  // Create Vertices
  osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(4);
  (*vertices)[0] = osg::Vec3f(-height / 2.0f, 0.0f, -width / 2.0f);
  (*vertices)[1] = osg::Vec3f(height / 2.0f, 0.0f, -width / 2.0f);
  (*vertices)[2] = osg::Vec3f(height / 2.0f, 0.0f, width / 2.0f);
  (*vertices)[3] = osg::Vec3f(-height / 2.0f, 0.0f, width / 2.0f);
  geometry->setVertexArray(vertices.get());

  // Create Normal
  osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array(1);
  (*normals)[0] = osg::Vec3f(0.0f, -1.0f, 0.0f);
  (*normals)[0].normalize();
  geometry->setNormalArray(normals.get());
  geometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

  // Create Vertex Color
  osg::ref_ptr<osg::Vec4Array> color = new osg::Vec4Array(4);
  (*color)[0].set(1.0f, 1.0f, 1.0f, 1.0f);
  (*color)[1].set(1.0f, 1.0f, 1.0f, 1.0f);
  (*color)[2].set(1.0f, 1.0f, 1.0f, 1.0f);
  (*color)[3].set(1.0f, 1.0f, 1.0f, 1.0f);
  geometry->setColorArray(color.get());
  geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

  // Create Texture Coordinate
  osg::ref_ptr<osg::Vec2Array> texture_coordinates = new osg::Vec2Array(4);
  (*texture_coordinates)[0].set(0.0f, 0.0f);
  (*texture_coordinates)[1].set(0.0f, 1.0f);
  (*texture_coordinates)[2].set(1.0f, 1.0f);
  (*texture_coordinates)[3].set(1.0f, 0.0f);
  geometry->setTexCoordArray(0, texture_coordinates.get());

  // Add Primitive (here a Quad)
  osg::ref_ptr<osg::DrawArrays> primitive =
      new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4);

  geometry->addPrimitiveSet(primitive.get());

  return geometry;
}
}
