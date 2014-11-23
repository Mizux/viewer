//! @file
//! @brief Main Window Widget

#ifndef VIEWER_TOOLS_GUI_HPP
#define VIEWER_TOOLS_GUI_HPP

#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QShortcut>

#include "image_viewer_2d_widget.hpp"
#include "image_viewer_3d_widget.hpp"

namespace Viewer {

/*! @brief Get Color Bar for normalized values (from 0.0 to 1.0)
 * @param[in] value Input in the range [0.0f; 1.0f], otherwise will be clamped.
 * @param[out] red Normalized red channel output.
 * @param[out] green Normalized green channel output.
 * @param[out] blue Normalized blue channel output.*/
inline void toColor(const float value, float *red, float *green, float *blue) {
  // Too Low
  if (0.0f > value) {
    *red = 0.0f;
    *green = 0.0f;
    *blue = 0.0f;
  }
  // Too High
  else if (1.0f < value) {
    *red = 1.0f;
    *green = 1.0f;
    *blue = 1.0f;
  }
  // Between [0.0;0.25]
  else if (0.25f >= value) {
    *red = 0.0f;
    *green = value * 4.0f;
    *blue = 1.0f;
  }
  // Between ]0.25;0.5]
  else if (0.50f >= value) {
    *red = 0.0f;
    *green = 1.0f;
    *blue = 1.0f - ((value - 0.25f) * 4.0f);
  }
  // Between ]0.5;0.75]
  else if (0.75f >= value) {
    *red = (value - 0.5f) * 4.0f;
    *green = 1.0f;
    *blue = 0.0f;
  }
  // Between ]0.75;1.0]
  else if (1.0f >= value) {
    *red = 1.0f;
    *green = 1.0f - ((value - 0.75f) * 4.0f);
    *blue = 0.0f;
  }
}

/// @class CToolsGUI.
/// @brief Main Window Widget
class ToolsGUI : public QWidget {
  Q_OBJECT

  // intentionally undefined
  ToolsGUI(const ToolsGUI &);
  // intentionally undefined
  ToolsGUI &operator=(const ToolsGUI &);

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
  Viewer::ImageViewer3DWidget *m_po3DImageViewerWidget;

  void SetupGUI();
};
}
#endif
