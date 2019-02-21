#include "graphicsviewdeprecated.h"
#include <QResizeEvent>

#include <QDebug>
GraphicsViewDeprecated::GraphicsViewDeprecated(QGraphicsView *parent)
  : QGraphicsView(parent),
    ctrlDown(false)
{

}

GraphicsViewDeprecated::~GraphicsViewDeprecated()
{

}

void GraphicsViewDeprecated::sceneFitInView()
{
  this->fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);

}

void GraphicsViewDeprecated::resizeEvent(QResizeEvent *event)
{
  if(ctrlDown)
    event->ignore();
  else
    {
      this->fitInView(scene()->sceneRect(), Qt::KeepAspectRatio);
//  scene()->setSceneRect(0, 0, event->size().width(), event->size().height());
      QGraphicsView::resizeEvent(event);
    }
}

void GraphicsViewDeprecated::mouseReleaseEvent(QMouseEvent *event)
{
  QPointF mousePoint = mapToScene( event->pos() );
  emit mouseClickOnScene(mousePoint);
  QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsViewDeprecated::mousePressEvent(QMouseEvent *event)
{
  QGraphicsView::mousePressEvent(event);
}

void GraphicsViewDeprecated::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Control)
    ctrlDown = true;
  QGraphicsView::keyPressEvent(event);
}

void GraphicsViewDeprecated::keyReleaseEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Control)
    ctrlDown = false;
  QGraphicsView::keyReleaseEvent(event);
}


void GraphicsViewDeprecated::wheelEvent(QWheelEvent* event)
{
  if(ctrlDown)
    {
      setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

      // Scale the view / do the zoom
      double scaleFactor = 1.15;
      if(event->delta() > 0) {
          // Zoom in
          scale(scaleFactor, scaleFactor);
        } else {
          // Zooming out
          scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
      // Don't call superclass handler here
      // as wheel is normally used for moving scrollbars
    }
  else
    QGraphicsView::wheelEvent(event);
}
