#include "graphicsview.hpp"
#include <QResizeEvent>
#include <QSettings>
#include <QScrollBar>
#include <QDebug>
GraphicsView::GraphicsView(const QString &configName, QWidget *parent)
  :
    QGraphicsView(parent)
  , ctrlDown(false)
  , configName(configName)
{
}

GraphicsView::~GraphicsView()
{

}

void GraphicsView::readSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsView");
  settings.beginGroup(configName);
  setTransform(settings.value("transform", QTransform()).value<QTransform>());

  QScrollBar *hBar = horizontalScrollBar();
  hBar->move(settings.value("horizontalScrollBar", QPoint()).toPoint());
  QScrollBar *vBar = verticalScrollBar();
  vBar->move(settings.value("verticalScrollBar", QPoint()).toPoint());
  settings.endGroup();
  settings.endGroup();
}

void GraphicsView::writeSettings()
{
  QSettings settings("Tweak", "ZErnest");
  settings.beginGroup("GraphicsView");
  settings.beginGroup(configName);
  settings.setValue("transform", transform());
  settings.setValue("horizontalScrollBar", horizontalScrollBar()->pos());
  settings.setValue("verticalScrollBar", verticalScrollBar()->pos());
  settings.endGroup();
  settings.endGroup();
}

void GraphicsView::sceneFitInView()
{
  this->fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
  if(ctrlDown)
    event->ignore();
  else
    {
      QGraphicsView::resizeEvent(event);
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  QPointF mousePoint = mapToScene( event->pos() );
  emit mouseClickOnScene(mousePoint);
  QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
  QGraphicsView::mousePressEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Control)
    ctrlDown = true;
  QGraphicsView::keyPressEvent(event);
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Control)
    ctrlDown = false;
  QGraphicsView::keyReleaseEvent(event);
}


void GraphicsView::wheelEvent(QWheelEvent* event)
{
  if(ctrlDown)
    {
      setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

      // Scale the view / do the zoom
      double scaleFactor = 1.15;
      if(event->delta() > 0) {
          scale(scaleFactor, scaleFactor);
        } else {
          scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
      // Don't call superclass handler here
      // as wheel is normally used for moving scrollbars
    }
  else
    QGraphicsView::wheelEvent(event);
}
