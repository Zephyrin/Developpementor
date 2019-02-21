#ifndef GRAPHICSVIEW_HPP
#define GRAPHICSVIEW_HPP


#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
  Q_OBJECT
public:
  GraphicsView(const QString &configName,
               QWidget *parent = NULL);
  void sceneFitInView();
  ~GraphicsView();
  void readSettings();
  void writeSettings();
signals:
  void mouseClickOnScene(const QPointF &);
protected:

  void resizeEvent(QResizeEvent *event);

  virtual void wheelEvent(QWheelEvent* event);
  // QWidget interface
  void mouseReleaseEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);

  void keyPressEvent(QKeyEvent *event);
  void keyReleaseEvent(QKeyEvent *event);

private:
  bool ctrlDown;
  QString configName;
};


#endif // GRAPHICSVIEW_H
