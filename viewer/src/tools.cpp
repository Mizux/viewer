//! @file
//! @brief Main Window Widget for the ViT Tools Application

#include <iostream>
#include "DataGrabberGUI/ToolsGUI.hpp"

#include "LogDefine.h"

using namespace DataGrabberGUI;

CToolsGUI::CToolsGUI(QWidget *parent) :
	QWidget(parent),
	m_qsExitShortcut(0),
	m_poLayout(0),
	m_poTabWidget(0),
	m_po2DImageViewerWidget(0),	
	m_po3DImageViewerWidget(0),	
	m_poQuitWidget(0)
{
#ifdef GUI_CTOOLSGUI_DEBUG
	std::cerr << "GUI_DEBUG : Constructor CToolsGUI" << std::endl;
#endif

	SetupGUI();
}

CToolsGUI::~CToolsGUI()
{
#ifdef GUI_CTOOLSGUI_DEBUG
	std::cerr << "GUI_DEBUG : Destructor CToolsGUI" << std::endl;
#endif
}

void CToolsGUI::SetupGUI()
{
	this->setObjectName(QString::fromUtf8("GUI"));
	m_poLayout = new QVBoxLayout(this);
	this->setLayout(m_poLayout);

	m_poTabWidget = new QTabWidget(this);
	m_poLayout->addWidget(m_poTabWidget);

	m_po2DImageViewerWidget = new C2DImageViewerWidget(this);
	m_poTabWidget->addTab(m_po2DImageViewerWidget, "2D Viewer");

	m_po3DImageViewerWidget = new C3DImageViewerWidget(this);
	m_poTabWidget->addTab(m_po3DImageViewerWidget, "3D Viewer");

	m_poQuitWidget = new CQuitWidget(this);
	m_poLayout->addWidget(m_poQuitWidget);

	m_qsExitShortcut = new QShortcut(Qt::CTRL + Qt::Key_Q, m_poQuitWidget, SLOT(slotQuit()));
}
