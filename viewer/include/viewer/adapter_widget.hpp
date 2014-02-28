/// @file

#ifndef ADAPTER_WIDGET_HPP
#define ADAPTER_WIDGET_HPP

#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>

#include <QGLWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
//#include <QString>

using Qt::WindowFlags;
#include <iostream>

class AdapterWidget : public QGLWidget
{
  public:
    AdapterWidget(
        QWidget * parent = 0,
        const QString& name = 0,
        const QGLWidget * shareWidget = 0,
        WindowFlags f = 0);

    virtual ~AdapterWidget() {}

    osgViewer::GraphicsWindow* getGraphicsWindow()
    {
      return _gw.get();
    }
    const osgViewer::GraphicsWindow* getGraphicsWindow() const
    {
      return _gw.get();
    }

  protected:
    void init();

    virtual void resizeGL(int width, int height);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> _gw;
};

class ViewerQT : public osgViewer::Viewer, public AdapterWidget
{
  public:
    ViewerQT(
        QWidget * parent = 0,
        const char * name = 0,
        const QGLWidget * shareWidget = 0,
        WindowFlags f = 0):
      AdapterWidget(parent, name, shareWidget, f), _timer()
    {
      getCamera()->setViewport(new osg::Viewport(0, 0, width(), height()));
      getCamera()->setProjectionMatrixAsPerspective(
            70.0f,
            static_cast<double>(width())/static_cast<double>(height()),
            1.0f, 10000.0f);
      getCamera()->setGraphicsContext(getGraphicsWindow());
      setThreadingModel(osgViewer::Viewer::SingleThreaded);

      connect(&_timer, SIGNAL(timeout()), this, SLOT(updateGL()));
      _timer.start(10);
    }

    virtual void paintGL()
    {
      frame();
    }

  protected:
    QTimer _timer;
};

#endif
/*EOF*/
