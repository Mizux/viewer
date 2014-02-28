#include <viewer/adapter_widget.hpp>

AdapterWidget::AdapterWidget(
    QWidget * parent,
    const QString& name,
    const QGLWidget * shareWidget,
    WindowFlags f):
  QGLWidget(parent, shareWidget, f), _gw()
{
  setObjectName(name);
  _gw = new osgViewer::GraphicsWindowEmbedded(0,0,width(),height());
  setFocusPolicy(Qt::ClickFocus);
}

void AdapterWidget::resizeGL( int width, int height )
{
  _gw->getEventQueue()->windowResize(0, 0, width, height );
  _gw->resized(0,0,width,height);
}

void AdapterWidget::keyPressEvent( QKeyEvent* event )
{
  _gw->getEventQueue()->keyPress( static_cast<char>(*(event->text().toAscii().data())) );
}

void AdapterWidget::keyReleaseEvent( QKeyEvent* event )
{
  _gw->getEventQueue()->keyRelease( static_cast<char>(*(event->text().toAscii().data())) );
}

void AdapterWidget::mousePressEvent( QMouseEvent* event )
{
  int button = 0;
  switch(event->button())
  {
    case(Qt::LeftButton): button = 1; break;
    case(Qt::MidButton): button = 2; break;
    case(Qt::RightButton): button = 3; break;
    case(Qt::NoButton): button = 0; break;
    default: button = 0; break;
  }
  _gw->getEventQueue()->mouseButtonPress(
        static_cast<float>(event->x()),
        static_cast<float>(event->y()),
        button);
}

void AdapterWidget::mouseReleaseEvent( QMouseEvent* event )
{
  int button = 0;
  switch(event->button())
  {
    case(Qt::LeftButton): button = 1; break;
    case(Qt::MidButton): button = 2; break;
    case(Qt::RightButton): button = 3; break;
    case(Qt::NoButton): button = 0; break;
    default: button = 0; break;
  }
  _gw->getEventQueue()->mouseButtonRelease(
        static_cast<float>(event->x()),
        static_cast<float>(event->y()),
        button);
}

void AdapterWidget::mouseMoveEvent( QMouseEvent* event )
{
  _gw->getEventQueue()->mouseMotion(
        static_cast<float>(event->x()),
        static_cast<float>(event->y()));
}

#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <QApplication>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>
int mainAdapterWidget(QApplication& a, osg::ArgumentParser& arguments)
{
  // load the scene.
  osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles(arguments);
  if (!loadedModel)
  {
    std::cerr << arguments[0] <<": No data loaded." << std::endl;
    std::cout << "usage: " << arguments[0] << " [--mdi] nodefile" << std::endl;
    return 1;
  }

  std::cout<<"Using AdapterWidget - QGLWidget subclassed to integrate with osgViewer using its embedded graphics window support."<<std::endl;

  if (arguments.read("--mdi"))
  {
    std::cout<<"Using ViewetQT MDI version"<<std::endl;
    /*
         Following problems are found here:
         - miminize causes loaded model to disappear (some problem with Camera matrix? - clampProjectionMatrix is invalid)
         */
    ViewerQT* viewerWindow = new ViewerQT;
    viewerWindow->setCameraManipulator(new osgGA::TrackballManipulator);
    viewerWindow->setSceneData(loadedModel.get());

    QMainWindow* mw = new QMainWindow();
    QMdiArea* mdiArea = new QMdiArea(mw);
    mw->setCentralWidget(mdiArea);

    QMdiSubWindow *subWindow = mdiArea->addSubWindow(viewerWindow);
    subWindow->showMaximized();
    subWindow->setWindowTitle("New Window");
    mw->show();
  }
  else
  {
    ViewerQT* viewerWindow = new ViewerQT;
    viewerWindow->setCameraManipulator(new osgGA::TrackballManipulator);
    viewerWindow->setSceneData(loadedModel.get());
    viewerWindow->show();
  }

  a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
  return a.exec();
}

/*EOF*/
