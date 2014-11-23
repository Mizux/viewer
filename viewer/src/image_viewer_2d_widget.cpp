//! @file

#include <viewer/image_viewer_2d_widget.hpp>

#include <QtGui/QFileDialog>
#include <QtGui/QSpacerItem>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

namespace Viewer {

ImageViewer2DWidget::ImageViewer2DWidget(QWidget *parent)
    : QWidget(parent), _loadButton(0), _imageViewer2D(0) {
  _setupWidget();
}

ImageViewer2DWidget::~ImageViewer2DWidget() {}

bool ImageViewer2DWidget::slotLoadImage(void) {
  QString fileName = QFileDialog::getOpenFileName(this, "Open Image File",
                                                  QDir(".").absolutePath());

  QImage image(fileName);
  _imageViewer2D->slotSet2DImage(image);

  return true;
}

void ImageViewer2DWidget::_setupWidget() {
  setObjectName("2D Image Viewer Widget");
  setLayout(new QVBoxLayout(this));

  _loadButton = new QPushButton(this);
  _loadButton->setText("Load 2D Image");
  _loadButton->setFixedSize(_loadButton->sizeHint());
  layout()->addWidget(_loadButton);

  _imageViewer2D = new Viewer::ImageViewer2D(this);
  _imageViewer2D->setMinimumSize(320, 240);
  _imageViewer2D->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  layout()->addWidget(_imageViewer2D);

  layout()->addItem(
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));

  QWidget::connect(_loadButton, SIGNAL(clicked()), SLOT(slotLoadImage()));
}
}
