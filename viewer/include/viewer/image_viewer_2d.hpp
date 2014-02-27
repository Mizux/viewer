/// @file

#ifndef _2D_IMAGE_VIEWER_HPP
#define _2D_IMAGE_VIEWER_HPP

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <Marshallable/Common/Image2D.h>

#include "DataGrabberGUI/OSGHeaders.hpp"
#include "DataGrabberGUI/AdapterWidget.hpp"

namespace DataGrabberGUI
{
	//! @class C2DImageViewer
	class C2DImageViewer : public ViewerQT
	{
		Q_OBJECT

			C2DImageViewer(const C2DImageViewer&);
		// intentionally undefined
		C2DImageViewer& operator=(const C2DImageViewer&);

		public:
			//! @brief Constructor.
			C2DImageViewer(QWidget *parent = 0);
	
			//! @brief Destructor.
			virtual ~C2DImageViewer();

			public slots:
			bool slotSet2DImage(
					const SharedData::Common::CImage2D& p_o2DImage,
					const bool p_bUseCoordinate=false);

			bool slotSet2DImages(
					const std::vector<SharedData::Common::CImage2D *>& p_arpo2DImages,
					const bool p_bUseCoordinate=false);

	private:
			bool addImage(
					const SharedData::Common::CImage2D& p_o2DImage,
					const bool p_bUseCoordinate=false);

		// Viewer OSG
		osg::ref_ptr<osg::Group> m_poRootNode;

		void SetupWidget();			
	};
} // DataGrabberGUI

#endif // end of include guard: 2D_IMAGE_VIEWER_HPP
