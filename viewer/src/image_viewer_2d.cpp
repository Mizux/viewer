/// @file

#include <Marshallable/Common/Image2DRAW.h>
#include <Marshallable/Common/Image2DRGB.h>
#include <Marshallable/Common/Image2DLuminance.h>

#include "LogDefine.h"
#include "DataGrabberGUI/ErrorMessage.hpp"
#include "DataGrabberGUI/2DImageViewer.hpp"


using namespace DataGrabberGUI;

C2DImageViewer::C2DImageViewer(QWidget *parent) :
	ViewerQT(parent),
	m_poRootNode(0)
{
#ifdef GUI_2DIMAGEVIEWER_DEBUG
	std::cerr << "GUI_DEBUG : Constructor C2DImageViewer" << std::endl;
#endif
	SetupWidget();
}

C2DImageViewer::~C2DImageViewer()
{
#ifdef GUI_2DIMAGEVIEWER_DEBUG
	std::cerr << "GUI_DEBUG : Destructor C2DImageViewer" << std::endl;
#endif
}

bool C2DImageViewer::slotSet2DImage(
		const SharedData::Common::CImage2D& p_o2DImage,
		const bool p_bUseCoordinate)
{
	//! Remove old Images Node if exist from Root Node.
	m_poRootNode->removeChildren(0, m_poRootNode->getNumChildren()) ;

	//! Build and Add New Image to Scene
	addImage(p_o2DImage, p_bUseCoordinate);

	return true;
}

bool C2DImageViewer::slotSet2DImages(
		const std::vector<SharedData::Common::CImage2D *>& p_arpo2DImages,
		const bool p_bUseCoordinate)
{
	//! Remove old Images Node if exist from Root Node.
	m_poRootNode->removeChildren(0, m_poRootNode->getNumChildren()) ;

	//! Build and Add New Images to Scene
	std::cerr << "2DViewer: Nb images to Display" << p_arpo2DImages.size() << std::endl;
	for (unsigned int uiLoop =0; uiLoop < p_arpo2DImages.size(); uiLoop++)
	{
		addImage(*(p_arpo2DImages[uiLoop]), p_bUseCoordinate);
	}

	return true;
}

bool C2DImageViewer::addImage(
		const SharedData::Common::CImage2D& p_o2DImage,
		const bool p_bUseCoordinate)
{
	using namespace SharedData::Common;
	
	//! Create Image object.
	osg::ref_ptr<osg::Image> poImage = new osg::Image;

	//! Create Texture and add it the Image object. 
	osg::ref_ptr<osg::Texture2D> poTexture2D = new osg::Texture2D;
	{
		poTexture2D->setDataVariance(osg::Object::STATIC);
		//! Use NEAREST Filter to avoid 2D Interpolation.
		poTexture2D->setFilter(
				osg::Texture2D::MIN_FILTER,
				osg::Texture2D::NEAREST);
		poTexture2D->setFilter(
				osg::Texture2D::MAG_FILTER,
				osg::Texture2D::NEAREST);

		//! Clamp Texture (no tiling).
		poTexture2D->setWrap(
				osg::Texture2D::WRAP_S,
				osg::Texture2D::CLAMP);
		poTexture2D->setWrap(
				osg::Texture2D::WRAP_T,
				osg::Texture2D::CLAMP);
		//! Keep Input Resolution no Interpolation to \f$2^n*2^m\f$.
		poTexture2D->setResizeNonPowerOfTwoHint(false);
		poTexture2D->setImage(poImage.get());
	}

	//! Create StateSet and add it the Texture object.
	osg::ref_ptr<osg::StateSet> poStateSet = new osg::StateSet;
	{
		poStateSet->setTextureAttributeAndModes(
				0,
				poTexture2D.get(),
				osg::StateAttribute::ON);
		//! Active Culling to not see the "back" of the texture.
		poStateSet->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
		//! Desactivate Lightning (we want to see the image color value!).
		poStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}

	//! Create Geode Object object.
	osg::ref_ptr<osg::Geode> poGeode = new osg::Geode;
	
	//! Create PAT Object object.
	osg::ref_ptr<osg::PositionAttitudeTransform> poPAT = new osg::PositionAttitudeTransform;
	poPAT->addChild(poGeode.get());

	//! Get Image Property
	int iWidth = p_o2DImage.GetWidth();
	int iHeight = p_o2DImage.GetHeight();

	//! Fill Image object with the 2D Image parameter
	if (IMAGE2D_RAW == p_o2DImage.GetType())
	{
		std::cerr << "2DViewer: RAW Image" << std::endl;
		CImage2DRAW &o2DImageRAW =
			const_cast<CImage2DRAW &>(
					static_cast<const CImage2DRAW &>(p_o2DImage));

		unsigned short usTmp;
		unsigned short *pusBuffer = new unsigned short[iWidth*iHeight];
		for (int iLoop=0; iLoop < iWidth*iHeight; iLoop++)
		{
			// ptr() return a char * pointer
			usTmp = *reinterpret_cast<unsigned short *>(
					o2DImageRAW.GetRefBayer().ptr()+2*iLoop);
			pusBuffer[iLoop] = usTmp << 6;
		}

		poImage->setImage(
				iWidth,
				iHeight,
				0,
				GL_LUMINANCE16,
				GL_LUMINANCE,
				GL_UNSIGNED_SHORT,
				reinterpret_cast<unsigned char *>(pusBuffer),
				osg::Image::USE_NEW_DELETE); 
		poTexture2D->dirtyTextureObject();
	}
	else if (IMAGE2D_RGB == p_o2DImage.GetType())
	{	
		std::cerr << "2DViewer: RGB Image" << std::endl;
		CImage2DRGB &o2DImageRGB =
			const_cast<CImage2DRGB &>(
					static_cast<const CImage2DRGB &>(p_o2DImage));

		unsigned char *pucBuffer = new unsigned char[iWidth*iHeight*3];
		for (int iLoop=0; iLoop < iWidth*iHeight; iLoop++)
		{
			// Write Red
			pucBuffer[3*iLoop+0] = 
				static_cast<unsigned char>(
						*(o2DImageRGB.GetRefRed().ptr()+iLoop));
			// Write Green
			pucBuffer[3*iLoop+1] = 
				static_cast<unsigned char>(
						*(o2DImageRGB.GetRefGreen().ptr()+iLoop));
			// Write Blue
			pucBuffer[3*iLoop+2] = 
				static_cast<unsigned char>(
						*(o2DImageRGB.GetRefBlue().ptr()+iLoop));
		}

		poImage->setImage(
				iWidth,
				iHeight,
				0,
				GL_RGB,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				pucBuffer,
				osg::Image::USE_NEW_DELETE); 
		poTexture2D->dirtyTextureObject();
	}
	else if (IMAGE2D_LUMINANCE == p_o2DImage.GetType())
	{	
		std::cerr << "2DViewer: Luminance Image" << std::endl;
		CImage2DLuminance &o2DImageLuminance =
			const_cast<CImage2DLuminance &>(
					static_cast<const CImage2DLuminance &>(p_o2DImage));

		unsigned char *pucBuffer = new unsigned char[iWidth*iHeight];
		for (int iLoop=0; iLoop < iWidth*iHeight; iLoop++)
		{
			pucBuffer[iLoop] = 
				static_cast<unsigned char>(
						*(o2DImageLuminance.GetRefLuminance().ptr()+iLoop));
		}

		poImage->setImage(
				iWidth,
				iHeight,
				0,
				GL_LUMINANCE,
				GL_LUMINANCE,
				GL_UNSIGNED_BYTE,
				pucBuffer,
				osg::Image::USE_NEW_DELETE); 
		poTexture2D->dirtyTextureObject();
	}
	else
	{
		std::cerr << "2DViwer: Image Type Unknow" << std::endl;
	}

	//! Create New image and add it to the Root Node.
	osg::ref_ptr<osg::Drawable> poDrawable;
	if (!p_bUseCoordinate)
	{
		//! Create Drawable object.
		poDrawable = CreateImage(iWidth, iHeight);
	}
	else
	{
		//! Place Image according to Parameters 
		float fRatio = 1000.0f;
		
		long lSensorIndex = 0;
		float fPositionX = 0.0f;
		float fPositionY = 0.0f;
		float fWidth = iWidth;
		float fHeight = iHeight;
		double dTmp;

		//! Read Image MetaData
		SharedData::Common::ImageMetaDataFacade oImageMetaData;
		oImageMetaData = p_o2DImage.GetImageMetaData();
		
		oImageMetaData.GetLongValue(
				IMAGE_CAMERA_INDEX,
				lSensorIndex);

		oImageMetaData.GetDoubleValue(
				IMAGE_ORIGIN_BAND_X,
				dTmp);
		fPositionX = dTmp;
		oImageMetaData.GetDoubleValue(
				IMAGE_ORIGIN_BAND_Y,
				dTmp);
		fPositionY = dTmp;

		oImageMetaData.GetDoubleValue(
				IMAGE_ORIGIN_WIDTH,
				dTmp);
		fWidth = dTmp;
		oImageMetaData.GetDoubleValue(
				IMAGE_ORIGIN_HEIGHT,
				dTmp);
		fHeight = dTmp;

		/*
		//! Display Values
		{
			std::cerr << "fPositionX: " << fPositionX << std::endl;
			std::cerr << "fPositionY: " << fPositionY << std::endl;
			std::cerr << "fWidth: " << fWidth << std::endl;
			std::cerr << "fHeight: " << fHeight << std::endl;
		}
		*/

		//! Update PositionAttitudeTranform Node object 
		if (0 == lSensorIndex || 1 == lSensorIndex)
		{
			poPAT->setPosition(
					osg::Vec3f(
						(fPositionX+fWidth/2.0f) / fRatio,
						0.0f,
						((fPositionY+fHeight/2.0f) / fRatio) - 50.0f));
		}
		else
		{
			poPAT->setPosition(
					osg::Vec3f(
						(fPositionX+fWidth/2.0f) / fRatio,
						0.0f,
						((fPositionY+fHeight/2.0f) / fRatio) + 50.0f));
		}

		//! Create Drawable object.
		poDrawable = 
			CreateImage(
					fHeight / fRatio,
					fWidth / fRatio);
	}

	//! Add StateSet to Drawable object
	poDrawable->setStateSet(poStateSet.get());

	//! Add Drawable object to Geode Object
	poGeode->addDrawable(poDrawable.get());
	
	//! Add PAT Node to the Root node.
	m_poRootNode->addChild(poPAT.get());
	
	//! Recompute Home to Center Object to Camera 
	this->getCameraManipulator()->home(0.0f);
	
	return true;		
}

void C2DImageViewer::SetupWidget()
{
	this->setObjectName("2D Image Viewer");
	
	// Trackball Manipulator
	this->setCameraManipulator(new osgGA::TrackballManipulator);
	// Stats Event Handler s key
	this->addEventHandler(new osgViewer::StatsHandler);
	// State Event Handler w,l key
	this->addEventHandler(
			new osgGA::StateSetManipulator(
				this->getCamera()->getOrCreateStateSet()));

	// Creating the Root node and add it the Geode object
	m_poRootNode = new osg::Group;

	// Select Root Node as The Scene.
	this->setSceneData(m_poRootNode.get()); 
	this->setMinimumSize(320,240);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
