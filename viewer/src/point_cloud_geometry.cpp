/// @file

#include <viewer/point_cloud_geometry.hpp>
#include <viewer/tools.hpp>

#include <osg/Geometry>
#include <osgUtil/DelaunayTriangulator>

#define RANGE_MAX 1.0f
#define RANGE_MIN -1.0f
#define MAX_POINT 150000

namespace Viewer
{

DrawablePtr createPointCloud(
    const float *pX,
    const float *pY,
    const float *pZ,
		unsigned int p_uiImageWidth,
		unsigned int p_uiImageHeight)
{
	//! <OL>
	//! <LI>Create Geometry Node.
	osg::ref_ptr<osg::Geometry> poGeometry  = new osg::Geometry;

	//! <LI>Create Vertice Array and Color Array.
	osg::ref_ptr<osg::Vec3Array> paroVertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> paroNormals = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec4Array> paroColors = new osg::Vec4Array();
	
	//! <LI>Setup Vertices Array.
	unsigned int uiCount=0;
	unsigned int uiLoop;

	int iStride=1;
	while (p_uiImageWidth*p_uiImageHeight/iStride/iStride > MAX_POINT)
	{
		iStride++;
	}

	unsigned int uiXmin = p_uiImageWidth/2 - p_uiImageWidth/iStride/2;
	unsigned int uiXmax = p_uiImageWidth/2 + p_uiImageWidth/iStride/2;
	unsigned int uiYmin = p_uiImageHeight/2 - p_uiImageHeight/iStride/2;
	unsigned int uiYmax = p_uiImageHeight/2 + p_uiImageHeight/iStride/2;
	
	for(unsigned int uiY=uiYmin; uiY < uiYmax/*p_uiImageHeight*/; uiY++/*=iStride*/)
	{
		for(unsigned int uiX=uiXmin; uiX < uiXmax/*p_uiImageWidth*/; uiX++/*=iStride*/)
		{
			uiLoop = uiY*p_uiImageWidth + uiX;
			
      if (isnormal(pX[uiLoop]) == 0 ||
          isnormal(pY[uiLoop]) == 0 ||
          isnormal(pZ[uiLoop]) == 0)
				continue;

      if (0.0f != pX[uiLoop] && 0.0f != pY[uiLoop] && RANGE_MIN <= pZ[uiLoop] && RANGE_MAX >= pZ[uiLoop])
			{
				uiCount++;
        paroVertices->push_back(osg::Vec3f(pX[uiLoop], pY[uiLoop], pZ[uiLoop]));
				paroNormals->push_back(osg::Vec3f(0.0f, 0.0f, 1.0f));
				paroColors->push_back(osg::Vec4f(1.0f,	1.0f,	1.0f,	1.0f));
			}
		}
	}

	//! <LI>Attach Vertices Array to Geometric Node.
	poGeometry->setVertexArray(paroVertices.get());

	//! <LI>Attach Normals Array to Geometric Node.
	poGeometry->setNormalArray(paroNormals.get());
	
	//! <LI>Compute Delaunay Triangulation
	{
		osg::ref_ptr<osgUtil::DelaunayTriangulator> poTriangulator = new osgUtil::DelaunayTriangulator(
				static_cast<osg::Vec3Array*>(poGeometry->getVertexArray()),
				static_cast<osg::Vec3Array*>(poGeometry->getNormalArray()));
		poTriangulator->triangulate();
		//! <LI>Attach Triangle List to Geometric Node.
		poGeometry->addPrimitiveSet(poTriangulator->getTriangles());
	}

	//! <LI>Correct Normal Vector to have all Vector Normal Z coordinate positif
	for(unsigned int uiLoop=0; uiLoop < paroNormals->size(); uiLoop++)
	{
		if (0.0f > (*paroNormals)[uiLoop][2])
			(*paroNormals)[uiLoop][2] = -(*paroNormals)[uiLoop][2];
	}

	//! <LI>Build Z Color Map 
	{
		// Scale Color
		float fMin = -0.5f;	
		float fMax = 0.5f;
		for(unsigned int uiLoop=0; uiLoop < paroVertices->size(); uiLoop++)
		{
      toColor(
					((*paroVertices)[uiLoop][2]-fMin)/(fMax-fMin),
					&((*paroColors)[uiLoop][0]),
					&((*paroColors)[uiLoop][1]),
					&((*paroColors)[uiLoop][2]));
		}
	}
	//! <LI>Attach Color Array to Geometric Node.
	poGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	poGeometry->setColorArray(paroColors.get());

	//! </OL>
	return poGeometry;
}

// this Structure will be use to sort good reconstruction points and keep their
// color values...
// Actually Delaunay Triangulation sort input vertices but not colors...
struct CPlop
{
	float fX;
	float fY;
	float fZ;
	unsigned char ucRed;
	unsigned char ucGreen;
	unsigned char ucBlue;

	//! @warning see Vec3f in OSG source code, it must be !!! THE SAME ALGORITHM !!! 
	bool operator()(
			struct CPlop p_oI,
			struct CPlop p_oJ)
	{ 
		if (p_oI.fX < p_oJ.fX) return true;
		else if (p_oI.fX > p_oJ.fX) return false;
		else if (p_oI.fY < p_oJ.fY) return true;
		else if (p_oI.fY > p_oJ.fY) return false;
		else return (p_oI.fZ < p_oJ.fZ);
	}
} myPlop;

DrawablePtr CreateTexturedPointCloud(
		const float *p_pfX,
		const float *p_pfY,
		const float *p_pfZ,
		const unsigned char * p_pucRed,
		const unsigned char * p_pucGreen,
		const unsigned char * p_pucBlue,
		unsigned int p_uiImageWidth,
		unsigned int p_uiImageHeight)
{
	//! <OL>
	//! <LI>Create Geometry Node.
	osg::ref_ptr<osg::Geometry> poGeometry  = new osg::Geometry;

	//! <LI>Create Vertice Array and Color Array.
	osg::ref_ptr<osg::Vec3Array> paroVertices = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec3Array> paroNormals = new osg::Vec3Array();
	osg::ref_ptr<osg::Vec4Array> paroColors = new osg::Vec4Array();
	std::vector<struct CPlop> aroPlop;
	struct CPlop oTmp;

	//! <LI>Setup Vertices Array.
	unsigned int uiCount=0;
	unsigned int uiLoop;

	std::cerr << "OSG: Total Point Number: " << p_uiImageWidth*p_uiImageHeight << std::endl;
	
	int iStride=1;
	unsigned int uiXmin = 0; 
	unsigned int uiXmax = p_uiImageWidth;
	unsigned int uiYmin = 0;
	unsigned int uiYmax = p_uiImageHeight;

	while (p_uiImageHeight*p_uiImageWidth/iStride/iStride > MAX_POINT)
	{
		iStride++;
	}
	std::cerr << "OSG: Stride: " << iStride << std::endl;
	
	for(unsigned int uiY=uiYmin; uiY < uiYmax; uiY+=iStride)
	{
		for(unsigned int uiX=uiXmin; uiX < uiXmax; uiX+=iStride)
		{
			uiLoop = uiY*p_uiImageWidth + uiX;

			if (isnormal(p_pfX[uiLoop]) == 0 ||
					isnormal(p_pfY[uiLoop]) == 0 ||
					isnormal(p_pfZ[uiLoop]) == 0)
				continue;

			if (0.0f != p_pfX[uiLoop] && 0.0f != p_pfY[uiLoop] && RANGE_MIN <= p_pfZ[uiLoop] && RANGE_MAX >= p_pfZ[uiLoop])
			{
				uiCount++;
				paroVertices->push_back(osg::Vec3f(p_pfX[uiLoop], p_pfY[uiLoop], p_pfZ[uiLoop]));
				paroNormals->push_back(osg::Vec3f(0.0f, 0.0f, 1.0f));
				paroColors->push_back(osg::Vec4f(1.0f,	1.0f,	1.0f,	1.0f));
				
				oTmp.fX = p_pfX[uiLoop];
				oTmp.fY = p_pfY[uiLoop];
				oTmp.fZ = p_pfZ[uiLoop];
				oTmp.ucRed = p_pucRed[uiLoop];
				oTmp.ucGreen = p_pucGreen[uiLoop];
				oTmp.ucBlue = p_pucBlue[uiLoop];
				aroPlop.push_back(oTmp);
			}
		}
	}

	// Debug
	/*
		 if(paroVertices->size() >= 8)
		 {
		 for (unsigned int uiLoop =0; uiLoop < 8; uiLoop++)
		 {
		 std::cerr <<
		 "[" << (*paroVertices)[uiLoop][0] <<
		 "," << (*paroVertices)[uiLoop][1] <<
		 "," << (*paroVertices)[uiLoop][2] <<
		 "]" << std::endl;
		 }
		 }
		 */

	//! <LI>Attach Vertices Array to Geometric Node.
	poGeometry->setVertexArray(paroVertices.get());

	//! <LI>Attach Normals Array to Geometric Node.
	poGeometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
	poGeometry->setNormalArray(paroNormals.get());

	//! <LI>Compute Delaunay Triangulation
	{
		std::cerr << "OSG: Epipolar Point Count Before Delaunay : " << uiCount << std::endl;

		osg::ref_ptr<osgUtil::DelaunayTriangulator> poTriangulator = new osgUtil::DelaunayTriangulator(
				static_cast<osg::Vec3Array*>(poGeometry->getVertexArray()),
				static_cast<osg::Vec3Array*>(poGeometry->getNormalArray()));
		poTriangulator->triangulate();
		//! <LI>Attach Triangle List to Geometric Node.
		poGeometry->addPrimitiveSet(poTriangulator->getTriangles());

		std::cerr << "OSG: Epipolar Point Count After Delaunay : " << paroVertices->size() << std::endl;
	}

	//! <LI>Correct Normal Vector to have all Vector Normal Z coordinate positif
	for(unsigned int uiLoop=0; uiLoop < paroNormals->size(); uiLoop++)
	{
		if (0.0f > (*paroNormals)[uiLoop][2])
			(*paroNormals)[uiLoop][2] = -(*paroNormals)[uiLoop][2];
	}

	//! <LI>Build Z Color Map 
	{
		std::sort(aroPlop.begin(), aroPlop.end(), myPlop);
		for(unsigned int uiVerticesLoop=0; uiVerticesLoop < paroVertices->size(); uiVerticesLoop++)
		{
			(*paroColors)[uiVerticesLoop][0] = (aroPlop[uiVerticesLoop].ucRed   / static_cast<float>(0xff));
			(*paroColors)[uiVerticesLoop][1] = (aroPlop[uiVerticesLoop].ucGreen / static_cast<float>(0xff));
			(*paroColors)[uiVerticesLoop][2] = (aroPlop[uiVerticesLoop].ucBlue  / static_cast<float>(0xff));
		}
	}
	//! <LI>Attach Color Array to Geometric Node.
	poGeometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	poGeometry->setColorArray(paroColors.get());

	//! </OL>
	return poGeometry;
}

}
