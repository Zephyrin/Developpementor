#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H


#include <QGraphicsView>

class GraphicsViewDeprecated : public QGraphicsView
{
  Q_OBJECT
public:
  GraphicsViewDeprecated(QGraphicsView *parent);
  void sceneFitInView();
  ~GraphicsViewDeprecated();
signals:
  void mouseClickOnScene(const QPointF &);
protected:
  void resizeEvent(QResizeEvent *event);

  virtual void wheelEvent(QWheelEvent* event);
  // QWidget interface
protected:
  void mouseReleaseEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);

  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

private:
  bool ctrlDown;
};


#endif // GRAPHICSVIEW_H
