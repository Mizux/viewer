//! @file
//! @brief Main Window Widget for the ViT Tools Application

#include <iostream>
#include <viewer/tools.hpp>

namespace Viewer {

ToolsGUI::ToolsGUI(QWidget *parent) :
	QWidget(parent),
	m_qsExitShortcut(0),
	m_poLayout(0),
	m_poTabWidget(0),
	m_po2DImageViewerWidget(0),	
  m_po3DImageViewerWidget(0)
{
	SetupGUI();
}

ToolsGUI::~ToolsGUI()
{
}

void ToolsGUI::SetupGUI()
{
	this->setObjectName(QString::fromUtf8("GUI"));
	m_poLayout = new QVBoxLayout(this);
	this->setLayout(m_poLayout);

	m_poTabWidget = new QTabWidget(this);
	m_poLayout->addWidget(m_poTabWidget);

  m_po2DImageViewerWidget = new ImageViewer2DWidget(this);
	m_poTabWidget->addTab(m_po2DImageViewerWidget, "2D Viewer");

	m_po3DImageViewerWidget = new ImageViewer3DWidget(this);
	m_poTabWidget->addTab(m_po3DImageViewerWidget, "3D Viewer");

  m_qsExitShortcut = new QShortcut(Qt::CTRL + Qt::Key_Q, this, SLOT(slotQuit()));
}

}
