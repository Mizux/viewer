//! @file
//! @brief Main Window Widget

#ifndef VIEWER_TOOLS_GUI_HPP
#define VIEWER_TOOLS_GUI_HPP

#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QShortcut>

#include "image_viewer_2d_widget.hpp"
#include "image_viewer_3d_widget.hpp"

namespace Viewer
{

/// @class CToolsGUI.
/// @brief Main Window Widget
class ToolsGUI : public QWidget
{
    Q_OBJECT

    // intentionally undefined
    ToolsGUI(const ToolsGUI&);
    // intentionally undefined
    ToolsGUI& operator =(const ToolsGUI&);

  public:
    /// @brief Constructor.
    ToolsGUI(QWidget *parent = 0);

    /// @brief Destructor.
    virtual ~ToolsGUI();

  private:
    QShortcut *m_qsExitShortcut;
    QVBoxLayout *m_poLayout;

    QTabWidget *m_poTabWidget;
    Viewer::ImageViewer2DWidget *m_po2DImageViewerWidget;
    Viewer::C3DImageViewerWidget *m_po3DImageViewerWidget;

    void SetupGUI();
};

}
#endif
