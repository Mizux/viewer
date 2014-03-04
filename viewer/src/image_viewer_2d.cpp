//! @file

#include <viewer/image_viewer_2d.hpp>

namespace Viewer
{

ImageViewer2D::ImageViewer2D(QWidget *parent) :
	ViewerQT(parent),
  _rootNode(0)
{
  _setupWidget();
}

ImageViewer2D::~ImageViewer2D()
{
}

bool ImageViewer2D::slotSet2DImage(const QImage& image)
{
  _rootNode->removeChildren(0, _rootNode->getNumChildren()) ;

  return _addImage(image);
}

bool ImageViewer2D::slotSet2DImages(const QVector<QImage>& images)
{
  _rootNode->removeChildren(0, _rootNode->getNumChildren()) ;

  bool result = true;
  foreach (const QVector<QImage>::value_type& it, images)
	{
    if (!_addImage(it))
      result = false;
	}

  return result;
}

bool ImageViewer2D::_addImage(const QImage& image)
{
	//! Create Image object.
  osg::ref_ptr<osg::Image> imagePtr = new osg::Image;

	//! Create Texture and add it the Image object. 
  osg::ref_ptr<osg::Texture2D> texture2dPtr = new osg::Texture2D;
	{
    texture2dPtr->setDataVariance(osg::Object::STATIC);
		//! Use NEAREST Filter to avoid 2D Interpolation.
    texture2dPtr->setFilter(
				osg::Texture2D::MIN_FILTER,
				osg::Texture2D::NEAREST);
    texture2dPtr->setFilter(
				osg::Texture2D::MAG_FILTER,
				osg::Texture2D::NEAREST);

		//! Clamp Texture (no tiling).
    texture2dPtr->setWrap(
				osg::Texture2D::WRAP_S,
				osg::Texture2D::CLAMP);
    texture2dPtr->setWrap(
				osg::Texture2D::WRAP_T,
        osg::Texture2D::CLAMP);

		//! Keep Input Resolution no Interpolation to \f$2^n*2^m\f$.
    texture2dPtr->setResizeNonPowerOfTwoHint(false);
    texture2dPtr->setImage(imagePtr.get());
	}

	//! Create StateSet and add it the Texture object.
  osg::ref_ptr<osg::StateSet> stateSetPtr = new osg::StateSet;
	{
    stateSetPtr->setTextureAttributeAndModes(
				0,
        texture2dPtr.get(),
				osg::StateAttribute::ON);
		//! Active Culling to not see the "back" of the texture.
    stateSetPtr->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
		//! Desactivate Lightning (we want to see the image color value!).
    stateSetPtr->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	//! Create PAT Object object.
  osg::ref_ptr<osg::PositionAttitudeTransform> patPtr = new osg::PositionAttitudeTransform;

  //! Create Geode Object object.
  osg::ref_ptr<osg::Geode> geodePtr = new osg::Geode;
  patPtr->addChild(geodePtr.get());

	//! Fill Image object with the 2D Image parameter
  if (QImage::Format_RGB16 == image.format())
	{
    unsigned char *buffer = new unsigned char[image.width()*image.height()*2];
    ::memcpy(buffer, image.bits(), image.width()*image.height()*2);

    imagePtr->setImage(
        image.width(),
        image.height(),
				0,
        GL_RGB,
        GL_RGB,
        GL_UNSIGNED_SHORT_5_6_5,
        buffer,
				osg::Image::USE_NEW_DELETE); 
	}
  else if (QImage::Format_RGB888 == image.format())
	{	
    unsigned char *buffer = new unsigned char[image.width()*image.height()*4];
    ::memcpy(buffer, image.bits(), image.width()*image.height()*4);

    imagePtr->setImage(
        image.width(),
        image.height(),
				0,
        GL_RGB,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        buffer,
				osg::Image::USE_NEW_DELETE); 
	}
  else if (QImage::Format_ARGB32 == image.format())
	{	
    unsigned char *buffer = new unsigned char[image.width()*image.height()*4];
    ::memcpy(buffer, image.bits(), image.width()*image.height()*4);

    imagePtr->setImage(
        image.width(),
        image.height(),
				0,
        GL_RGBA,
        GL_RGBA,
        GL_UNSIGNED_INT_8_8_8_8,
        buffer,
				osg::Image::USE_NEW_DELETE); 
	}
	else
	{
    std::cerr << "Image Format unsupported" << std::endl;
    return false;
  }
  texture2dPtr->dirtyTextureObject();

	//! Create New image and add it to the Root Node.
  osg::ref_ptr<osg::Drawable> drawablePtr;
  drawablePtr = createDrawable(static_cast<float>(image.width()),
                            static_cast<float>(image.height()));

	//! Add StateSet to Drawable object
  drawablePtr->setStateSet(stateSetPtr.get());

	//! Add Drawable object to Geode Object
  geodePtr->addDrawable(drawablePtr.get());
	
	//! Add PAT Node to the Root node.
  _rootNode->addChild(patPtr.get());
	
	//! Recompute Home to Center Object to Camera 
  getCameraManipulator()->home(0.0f);
	return true;		
}

void ImageViewer2D::_setupWidget()
{
  setObjectName("2D Image Viewer");
	
	// Trackball Manipulator
  setCameraManipulator(new osgGA::TrackballManipulator);
	// Stats Event Handler s key
  addEventHandler(new osgViewer::StatsHandler);
	// State Event Handler w,l key
  addEventHandler(new osgGA::StateSetManipulator(getCamera()->getOrCreateStateSet()));

	// Creating the Root node and add it the Geode object
  _rootNode = new osg::Group;

	// Select Root Node as The Scene.
  setSceneData(_rootNode.get());
  setMinimumSize(320,240);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

}
