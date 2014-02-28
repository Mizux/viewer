/// @file

#ifndef OSG_HEADERS_HPP
#define OSG_HEADERS_HPP

#include <osg/ref_ptr>
#include <osg/Notify>

// OSG library
#include <osg/Node>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/Image>
#include <osg/Material>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/Point>
#include <osg/PointSprite>
#include <osg/PositionAttitudeTransform>
// OSG DB (files I/O)
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

// OSG Viewer
#include <osg/Timer>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventHandler>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>

#include "image_geometry.hpp"

//! @brief Get Color Bar for normalized values (from 0.0 to 1.0)
inline bool GetColor(const float p_fValue, float *p_pfRed, float *p_pfGreen, float *p_pfBlue)
{
	// Too Low
	if (0.0f > p_fValue)
	{
		*p_pfRed = 0.0f;
		*p_pfGreen = 0.0f;
		*p_pfBlue = 0.0f;
	}
	// Too High
	else if (1.0f < p_fValue)
	{
		*p_pfRed = 1.0f;
		*p_pfGreen = 1.0f;
		*p_pfBlue = 1.0f;
	}
	// Between [0.0;0.25] 
	else if (0.25f >= p_fValue)
	{
		*p_pfRed = 0.0f;
		*p_pfGreen = p_fValue * 4.0f;
		*p_pfBlue = 1.0f;
	}
	// Between ]0.25;0.5] 
	else if (0.50f >= p_fValue)
	{
		*p_pfRed = 0.0f;
		*p_pfGreen = 1.0f;
		*p_pfBlue = 1.0f - ((p_fValue-0.25f)*4.0f);
	}
	// Between ]0.5;0.75] 
	else if (0.75f >= p_fValue)
	{
		*p_pfRed = (p_fValue-0.5f) * 4.0f;
		*p_pfGreen = 1.0f;
		*p_pfBlue = 0.0f;
	}
	// Between ]0.75;1.0] 
	else if (1.0f >= p_fValue)
	{
		*p_pfRed = 1.0f;
		*p_pfGreen = 1.0f - ((p_fValue-0.75f)*4.0f);
		*p_pfBlue = 0.0f;
	}

	return true;
}

#endif
