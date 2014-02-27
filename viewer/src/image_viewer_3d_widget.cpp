/// @file 3DImageViewerWidget.cpp

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "LogDefine.h"
#include "DataGrabberGUI/ErrorMessage.hpp"
#include "DataGrabberGUI/3DImageViewerWidget.hpp"

using namespace DataGrabberGUI;

C3DImageViewerWidget::C3DImageViewerWidget(QWidget *parent) :
	QWidget(parent),
	m_poLayout(0),
	m_poLoadButton(0),
	m_po3DImageViewer(0)
{
#ifdef GUI_3DIMAGEVIEWERWIDGET_DEBUG
	std::cerr << "GUI_DEBUG : Constructor C3DImageViewerWidget" << std::endl;
#endif
	SetupWidget();
}

C3DImageViewerWidget::~C3DImageViewerWidget()
{
#ifdef GUI_3DIMAGEVIEWERWIDGET_DEBUG
	std::cerr << "GUI_DEBUG : Destructor C3DImageViewerWidget" << std::endl;
#endif
}

bool C3DImageViewerWidget::slotLoadImage(void)
{
	// Specify which Sequence File the user wants to load
	QString strFileName = QFileDialog::getOpenFileName(
			this,
			"Open 3D Image File",
			QDir(".").absolutePath(),
			"Serialized Object (*.bin)");

	SharedData::Common::CImage3D* poImage3DPtr;
	SharedData::Common::CImage3D poImage3DObj;
	struct stat oFileStat;
	
	if (stat(strFileName.toStdString().c_str(), &oFileStat) == 0)
	{
		try
		{
			std::ifstream oFile;
			oFile.open(strFileName.toStdString().c_str());
			boost::archive::binary_iarchive ia(oFile, boost::archive::no_header);
			ia >> poImage3DPtr;
			m_po3DImageViewer->slotSet3DImage(*poImage3DPtr);
			delete poImage3DPtr;
			oFile.close();
		}
		catch(...)
		{
			try
			{
				std::ifstream oFile;
				oFile.open(strFileName.toStdString().c_str());
				boost::archive::binary_iarchive ia(oFile, boost::archive::no_header);
				ia >> poImage3DObj;
				m_po3DImageViewer->slotSet3DImage(poImage3DObj);
				oFile.close();
			}
			catch(...)
			{
				QMessageBox oMsgBox;
				oMsgBox.setText("Error: file does not contains a CImage3D");
				oMsgBox.exec();

				return false;
			}
		}
	}
	
	return true;
}

void C3DImageViewerWidget::SetupWidget()
{
	this->setObjectName("3D Image Viewer Widget");
	// Widget
	m_poLayout = new QVBoxLayout(this);
	this->setLayout(m_poLayout);
	
	m_poLoadButton = new QPushButton(this);
	m_poLoadButton->setText("Load 3D Image");
	m_poLoadButton->setFixedSize(m_poLoadButton->sizeHint());
	m_poLayout->addWidget(m_poLoadButton);

	m_po3DImageViewer = new DataGrabberGUI::C3DImageViewer(this);
	m_po3DImageViewer->setMinimumSize(320,240);
	m_po3DImageViewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_poLayout->addWidget(m_po3DImageViewer);

	QWidget::connect(
			m_poLoadButton,
			SIGNAL(clicked()),
			SLOT(slotLoadImage()));
}
