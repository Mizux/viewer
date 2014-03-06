//! @file

#include <viewer/image_viewer_3d_widget.hpp>

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <osgDB/ReadFile>

namespace Viewer
{

ImageViewer3DWidget::ImageViewer3DWidget(QWidget *parent) :
  QWidget(parent),
  _loadButton(0),
  _imageViewer3D(0)
{
  _setupWidget();
}

ImageViewer3DWidget::~ImageViewer3DWidget()
{
}

bool ImageViewer3DWidget::slotLoadImage(void)
{
  QString fileName = QFileDialog::getOpenFileName(this, "Open Node File", QDir(".").absolutePath());

  osg::Node * tmp = osgDB::readNodeFile(fileName.toStdString());
  //! @bug not working...
  //_imageViewer3D->slotSet3DImage(tmp);

  return true;
}

void ImageViewer3DWidget::_setupWidget()
{
  setObjectName("3D Image Viewer Widget");
  setLayout(new QVBoxLayout(this));

  _loadButton = new QPushButton(this);
  _loadButton->setText("Load 3D Image");
  _loadButton->setFixedSize(_loadButton->sizeHint());
  layout()->addWidget(_loadButton);

  _imageViewer3D = new ImageViewer3D(this);
  _imageViewer3D->setMinimumSize(320,240);
  _imageViewer3D->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout()->addWidget(_imageViewer3D);

  QWidget::connect(
        _loadButton,
        SIGNAL(clicked()),
        SLOT(slotLoadImage()));
}

}
