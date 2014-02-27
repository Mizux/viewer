/// @file 2DImageViewerWidget.hpp

#ifndef _2D_IMAGE_VIEWER_WIDGET_HPP
#define _2D_IMAGE_VIEWER_WIDGET_HPP

#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>

#include "DataGrabberGUI/2DImageViewer.hpp"
#include "DataGrabberGUI/OSGHeaders.hpp"
#include "DataGrabberGUI/AdapterWidget.hpp"

namespace DataGrabberGUI
{
	//! @class C2DImageViewerWidget
	class C2DImageViewerWidget : public QWidget
	{
		Q_OBJECT

			C2DImageViewerWidget(const C2DImageViewerWidget&);
		// intentionally undefined
		C2DImageViewerWidget& operator=(const C2DImageViewerWidget&);

		public:
			//! @brief Constructor.
			C2DImageViewerWidget(QWidget *parent = 0);
	
			//! @brief Destructor.
			virtual ~C2DImageViewerWidget();

			public slots:
			bool slotLoadImage(void);

	private:
		QVBoxLayout *m_poLayout;

		QPushButton *m_poLoadButton;
        QCheckBox *m_poCoordinatesCheckBox;
		DataGrabberGUI::C2DImageViewer *m_po2DImageViewer;

		void SetupWidget();			
	};
} // DataGrabberGUI

#endif // end of include guard: 2D_IMAGE_VIEWER_HPP
