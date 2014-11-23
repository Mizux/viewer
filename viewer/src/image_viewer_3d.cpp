//! @file

#include <viewer/image_viewer_3d.hpp>

#include <osg/Point>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgViewer/ViewerEventHandlers>

namespace Viewer {

ImageViewer3D::ImageViewer3D(QWidget* parent) : ViewerQT(parent), _rootNode(0) {
  _setupWidget();
}

ImageViewer3D::~ImageViewer3D() {}

bool ImageViewer3D::slotSet3DImage(const DrawablePtr& image) {
  _rootNode->removeChildren(0, _rootNode->getNumChildren());

  return addImage(image);
}

bool ImageViewer3D::slotSet3DImages(const DrawablePtrArray& images) {
  _rootNode->removeChildren(0, _rootNode->getNumChildren());

  bool result = true;
  foreach (const DrawablePtrArray::value_type& it, images) {
    if (!addImage(it)) result = false;
  }

  return result;
}

bool ImageViewer3D::addImage(const DrawablePtr& image) {
  //! Create StateSet and add it the Point object.
  osg::ref_ptr<osg::StateSet> stateSetPtr = new osg::StateSet;
  {
    //! Add Point Attribute.
    osg::ref_ptr<osg::Point> point = new osg::Point();
    point->setSize(1.0f);
    stateSetPtr->setAttribute(point.get());
    //! Activate The Depth Test.
    stateSetPtr->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
    //! Active Culling to not see the "back" of the texture.
    stateSetPtr->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
    //! Desactivate Lightning (we want to see the image color value!).
    stateSetPtr->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  }

  //! Create Geode Object and add it the Drawable object.
  osg::ref_ptr<osg::Geode> geodePtr = new osg::Geode;
  geodePtr->setStateSet(stateSetPtr);
  geodePtr->addDrawable(image.get());
  _rootNode->addChild(geodePtr.get());
  //! Recompute Home to Center Object to Camera
  getCameraManipulator()->home(0.0f);

  return true;
}

void ImageViewer3D::_setupWidget() {
  setObjectName("3D Image Viewer");
  setMinimumSize(320, 240);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // Trackball Manipulator
  setCameraManipulator(new osgGA::TrackballManipulator);
  // Stats Event Handler s key
  addEventHandler(new osgViewer::StatsHandler);
  // State Event Handler w,l key
  addEventHandler(
      new osgGA::StateSetManipulator(getCamera()->getOrCreateStateSet()));

  // Creating the Root node and add it the Geode object
  _rootNode = new osg::Group;
  setSceneData(_rootNode.get());
}
}
