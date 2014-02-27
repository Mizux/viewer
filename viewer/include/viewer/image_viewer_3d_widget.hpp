/// @file 3DImageViewerWidget.hpp

#ifndef _3D_IMAGE_VIEWER_WIDGET_HPP
#define _3D_IMAGE_VIEWER_WIDGET_HPP

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include "DataGrabberGUI/3DImageViewer.hpp"
#include "DataGrabberGUI/OSGHeaders.hpp"
#include "DataGrabberGUI/AdapterWidget.hpp"

namespace DataGrabberGUI
{
	//! @class C3DImageViewerWidget
	class C3DImageViewerWidget : public QWidget
	{
		Q_OBJECT

			C3DImageViewerWidget(const C3DImageViewerWidget&);
		// intentionally undefined
		C3DImageViewerWidget& operator=(const C3DImageViewerWidget&);

		public:
			//! @brief Constructor.
			C3DImageViewerWidget(QWidget *parent = 0);
	
			//! @brief Destructor.
			virtual ~C3DImageViewerWidget();

			public slots:
			bool slotLoadImage(void);

	private:
		QVBoxLayout *m_poLayout;

		QPushButton *m_poLoadButton;
		DataGrabberGUI::C3DImageViewer *m_po3DImageViewer;

		void SetupWidget();			
	};
} // DataGrabberGUI

#endif // end of include guard: 3D_IMAGE_VIEWER_HPP
