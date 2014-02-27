/// @file 2DImageViewerWidget.cpp

#include <QtGui/QFileDialog>
#include <QtGui/QSpacerItem>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "LogDefine.h"
#include "DataGrabberGUI/ErrorMessage.hpp"
#include "DataGrabberGUI/2DImageViewerWidget.hpp"

using namespace DataGrabberGUI;

C2DImageViewerWidget::C2DImageViewerWidget(QWidget *parent) :
	QWidget(parent),
	m_poLayout(0),
	m_poLoadButton(0),
	m_poCoordinatesCheckBox(0),
	m_po2DImageViewer(0)
{
#ifdef GUI_2DIMAGEVIEWERWIDGET_DEBUG
	std::cerr << "GUI_DEBUG : Constructor C2DImageViewerWidget" << std::endl;
#endif
	SetupWidget();
}

C2DImageViewerWidget::~C2DImageViewerWidget()
{
#ifdef GUI_2DIMAGEVIEWERWIDGET_DEBUG
	std::cerr << "GUI_DEBUG : Destructor C2DImageViewerWidget" << std::endl;
#endif
}

bool C2DImageViewerWidget::slotLoadImage(void)
{
	// Specify which Sequence File the user wants to load
	QString strFileName = QFileDialog::getOpenFileName(
			this,
			"Open 2D Image File",
			QDir(".").absolutePath(),
			"Serialized Object (*.bin)");

	SharedData::Common::CImage2D* poImage2D;
	struct stat oFileStat;
	
	if (stat(strFileName.toStdString().c_str(), &oFileStat) == 0)
	{
		std::ifstream oFile;
		oFile.open(strFileName.toStdString().c_str());
		
		boost::archive::binary_iarchive ia(oFile, boost::archive::no_header);
		ia >> poImage2D;
		m_po2DImageViewer->slotSet2DImage(*poImage2D, m_poCoordinatesCheckBox->isChecked());
		delete poImage2D;

		oFile.close();
	}
	
	return true;
}

void C2DImageViewerWidget::SetupWidget()
{
    QSpacerItem *poSpacer;    

	this->setObjectName("2D Image Viewer Widget");
	// Widget
	m_poLayout = new QVBoxLayout(this);
	this->setLayout(m_poLayout);
	
	m_poLoadButton = new QPushButton(this);
	m_poLoadButton->setText("Load 2D Image");
	m_poLoadButton->setFixedSize(m_poLoadButton->sizeHint());
	m_poLayout->addWidget(m_poLoadButton);

	poSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	m_poLayout->addItem(poSpacer);

    m_poCoordinatesCheckBox = new QCheckBox("Represent in geometric coordinates system", this);
    m_poLayout->addWidget(m_poCoordinatesCheckBox);

	m_po2DImageViewer = new DataGrabberGUI::C2DImageViewer(this);
	m_po2DImageViewer->setMinimumSize(320,240);
	m_po2DImageViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_poLayout->addWidget(m_po2DImageViewer);

	QWidget::connect(
			m_poLoadButton,
			SIGNAL(clicked()),
			SLOT(slotLoadImage()));
}
