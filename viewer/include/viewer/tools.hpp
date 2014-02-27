//! @file
//! @brief Main Window Widget for the ViT Tools Application
//! Header File.

#ifndef TOOLS_GUI_HPP
#define TOOLS_GUI_HPP

#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QShortcut>

#include "2DImageViewerWidget.hpp"
#include "3DImageViewerWidget.hpp"

#include "QuitWidget.hpp"

namespace DataGrabberGUI
{
  /// @class CToolsGUI.
  /// @brief Main Window Widget 
  class CToolsGUI : public QWidget
	{
	  Q_OBJECT

	  // intentionally undefined
    CToolsGUI(const CToolsGUI&);
	  // intentionally undefined
	  CToolsGUI& operator =(const CToolsGUI&);

    public :

    /// @brief Constructor.
    CToolsGUI(QWidget *parent = 0);

    /// @brief Destructor. 
    virtual ~CToolsGUI();

    private:
		QShortcut *m_qsExitShortcut;
		QVBoxLayout *m_poLayout;

		QTabWidget *m_poTabWidget;
		DataGrabberGUI::C2DImageViewerWidget *m_po2DImageViewerWidget;
		DataGrabberGUI::C3DImageViewerWidget *m_po3DImageViewerWidget;

		DataGrabberGUI::CQuitWidget *m_poQuitWidget;
    
		void SetupGUI();
  };
}
#endif
