#include "DataGrabberGUI/AdapterWidget.hpp"

AdapterWidget::AdapterWidget( QWidget * parent, const char * /*name*/, const QGLWidget * shareWidget, WindowFlags f):
    QGLWidget(parent, shareWidget, f), _gw()
{
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
    _gw->getEventQueue()->mouseButtonPress(event->x(), event->y(), button);
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
    _gw->getEventQueue()->mouseButtonRelease(event->x(), event->y(), button);
}

void AdapterWidget::mouseMoveEvent( QMouseEvent* event )
{
    _gw->getEventQueue()->mouseMotion(event->x(), event->y());
}

int mainAdapterWidget(QApplication& a, osg::ArgumentParser& arguments)
{
    // load the scene.
    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFiles(arguments);
    if (!loadedModel)
    {
        std::cout << arguments[0] <<": No data loaded." << std::endl;
        return 1;
    }
    
    std::cout<<"Using AdapterWidget - QGLWidget subclassed to integrate with osgViewer using its embedded graphics window support."<<std::endl;
    
    if (arguments.read("--CompositeViewer"))
    {
        CompositeViewerQT* viewerWindow = new CompositeViewerQT;

        unsigned int width = viewerWindow->width();
        unsigned int height = viewerWindow->height();
        
        {
            osgViewer::View* view1 = new osgViewer::View;
            view1->getCamera()->setGraphicsContext(viewerWindow->getGraphicsWindow());
            view1->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width)/static_cast<double>(height/2), 1.0, 1000.0);
            view1->getCamera()->setViewport(new osg::Viewport(0,0,width,height/2));
            view1->setCameraManipulator(new osgGA::TrackballManipulator);
            view1->setSceneData(loadedModel.get());
            
            viewerWindow->addView(view1);
        }
        
        {
            osgViewer::View* view2 = new osgViewer::View;
            view2->getCamera()->setGraphicsContext(viewerWindow->getGraphicsWindow());
            view2->getCamera()->setProjectionMatrixAsPerspective(30.0f, static_cast<double>(width)/static_cast<double>(height/2), 1.0, 1000.0);
            view2->getCamera()->setViewport(new osg::Viewport(0,height/2,width,height/2));
            view2->setCameraManipulator(new osgGA::TrackballManipulator);
            view2->setSceneData(loadedModel.get());
            
            viewerWindow->addView(view2);
        }

        viewerWindow->show();
    }
    else if (arguments.read("--mdi")) {
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
    } else {
        ViewerQT* viewerWindow = new ViewerQT;

        viewerWindow->setCameraManipulator(new osgGA::TrackballManipulator);
        viewerWindow->setSceneData(loadedModel.get());

        viewerWindow->show();
    }    
    
    
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
   
    return a.exec();
}

/*EOF*/
