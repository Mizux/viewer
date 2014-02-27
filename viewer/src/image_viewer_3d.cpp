/// @file

#include <iostream>
#include <sstream>
#include "LogDefine.h"
#include "DataGrabberGUI/ErrorMessage.hpp"
#include "DataGrabberGUI/3DImageViewer.hpp"

using namespace DataGrabberGUI;

C3DImageViewer::C3DImageViewer(QWidget *parent) :
	ViewerQT(parent),
	m_poRootNode(0)
{
#ifdef GUI_3DIMAGEVIEWER_DEBUG
	std::cerr << "GUI_DEBUG : Constructor C3DImageViewer" << std::endl;
#endif
	SetupWidget();
}

C3DImageViewer::~C3DImageViewer()
{
#ifdef GUI_3DIMAGEVIEWER_DEBUG
	std::cerr << "GUI_DEBUG : Destructor C3DImageViewer" << std::endl;
#endif
}

bool C3DImageViewer::slotSet3DImage(
		const SharedData::Common::CImage3D& p_o3DImage)
{
	//! Remove old Images Node if exist from Root Node.
	m_poRootNode->removeChildren(0, m_poRootNode->getNumChildren()) ;

	//! Build and Add New Image to Scene
	addImage(p_o3DImage);

	return true;
}

bool C3DImageViewer::slotSet3DImages(
		const std::vector<SharedData::Common::CImage3D *>& p_arpo3DImages)
{
	//! Remove old Images Node if exist from Root Node.
	m_poRootNode->removeChildren(0, m_poRootNode->getNumChildren()) ;

	//! Build and Add New Images to Scene
	for (unsigned int uiLoop =0; uiLoop < p_arpo3DImages.size(); uiLoop++)
	{
		addImage(*(p_arpo3DImages[uiLoop]));
	}

	return true;
}

bool C3DImageViewer::addImage(const SharedData::Common::CImage3D& p_o3DImage)
{
	using namespace SharedData::Common;

	//! Create Point attribute.
	osg::ref_ptr<osg::Point> poPoint = new osg::Point();
	poPoint->setSize(1.0f);

	//! Create StateSet and add it the Point object.
	osg::ref_ptr<osg::StateSet> poStateSet = new osg::StateSet;
	{
		//! Add Point Attribute.
		poStateSet->setAttribute(poPoint.get());
		//! Activate The Depth Test.
		poStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
		//! Active Culling to not see the "back" of the texture.
		poStateSet->setMode(GL_CULL_FACE, osg::StateAttribute::OFF);
		//! Desactivate Lightning (we want to see the image color value!).
		poStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	}
	
	//! Get Image Property
	int iWidth = p_o3DImage.GetCols();
	int iHeight = p_o3DImage.GetRows();

	//! Create Drawable object and add it the StateSet object.
	osg::ref_ptr<osg::Drawable> poDrawable; 
	{

		SharedData::Common::CTextureRGB oTexture;
		if (!p_o3DImage.GetTexture(oTexture))
		{
			poDrawable = 
				CreatePointCloud(
						&(p_o3DImage.GetX()[0]),
						&(p_o3DImage.GetY()[0]),
						&(p_o3DImage.GetZ()[0]),
						iWidth,
						iHeight);
		}
		else
		{
			poDrawable = 
				CreateTexturedPointCloud(
						&(p_o3DImage.GetX()[0]),
						&(p_o3DImage.GetY()[0]),
						&(p_o3DImage.GetZ()[0]),
						&(oTexture.GetRed()[0]),
						&(oTexture.GetGreen()[0]),
						&(oTexture.GetBlue()[0]),
						iWidth,
						iHeight);
		}
		poDrawable->setStateSet(poStateSet.get());
	}

	//! Create Geode Object and add it the Drawable object.
	osg::ref_ptr<osg::Geode> poGeode = new osg::Geode;
	poGeode->addDrawable(poDrawable.get());

	//! Add Geode Node to the Root node.
	m_poRootNode->addChild(poGeode.get());

	//! Recompute Home to Center Object to Camera 
	this->getCameraManipulator()->home(0.0f);

	return true;
}

void C3DImageViewer::SetupWidget()
{
	this->setObjectName("3D Image Viewer");

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
