#include "environmentsmallloopwidget.h"
#include "ui_environmentsmallloopwidget.h"
#include <QDebug>

EnvironmentSmallLoopWidget::EnvironmentSmallLoopWidget(
    EnvironmentSmallLoop *loop,
    QWidget *parent) :
  QWidget(parent),
  ui(new Ui::EnvironmentSmallLoopWidget),
  X(0),
  Y(0),
  lastFeel(NULL),
  loop(loop)
{
  ui->setupUi(this);


  view = new GraphicsView("smallloop", NULL);
  view->setScene(&scene);
  view->setRenderHints(QPainter::Antialiasing
                       | QPainter::SmoothPixmapTransform);
  connect(view, SIGNAL(mouseClickOnScene(const QPointF &)), this,
          SLOT(mouseClickOnScene(const QPointF &)));
  ui->horizontalLayout->addWidget(view);
  connect(loop, SIGNAL(feelSignal(const int, const int)),
          this, SLOT(feelSlot(const int, const int)));
  connect(loop, SIGNAL(moveSignal(const unsigned int, const int)),
          this, SLOT(moveSlot(const unsigned int, const int)));
  connect(loop,SIGNAL(turnSignal(const int)),
          this, SLOT(turnSlot(const int)));
  connect(this, SIGNAL(changeItem(const int)),
          loop, SLOT(changeItem(const int)));
  connect(loop, SIGNAL(initFinished()), this, SLOT(init()));
}

EnvironmentSmallLoopWidget::~EnvironmentSmallLoopWidget()
{
  delete ui;
}

void EnvironmentSmallLoopWidget::init()
{
  X = loop->getX();
  Y = loop->getY();
  const std::vector<EnvironmentSmallLoop::LOOP> &l = loop->getLoop();
  scene.setSceneRect(X, Y, 0, 0);
  scene.setBackgroundBrush(QBrush(Qt::lightGray));
  this->boards.resize(l.size());
  for(std::size_t j = 0; j < Y; j ++)
    {
      for(std::size_t i = 0; i < X; i ++)
        {
          this->boards[i + j * X] = scene.addRect(0, 0, 1,
                                               0,
                                               l.at(i + j * X)
                                               == EnvironmentSmallLoop::EMPTY ?
                                                 QPen(Qt::white)
                                               : QPen(Qt::black));
          this->boards[i + j * X]->setPos(i, j);
        }
    }
  QPolygonF p;
  p.clear();
  p.append(QPointF(0., 0.));
  p.append(QPointF(30., 60.));
  p.append(QPointF(60., 0.));
  p.append(QPointF(30., 20.));

  agent = scene.addPolygon(p, QPen(Qt::black, 1), QBrush(Qt::blue) );
  cLeft = scene.addRect(42., 20., 10, 10, QPen(Qt::black, 1),
                        QBrush(Qt::green));
  cRight = scene.addRect(8., 20., 10, 10, QPen(Qt::black, 1),
                         QBrush(Qt::yellow));
  cBehind = scene.addRect(25, 55., 10, 10, QPen(Qt::black, 1),
                          QBrush(Qt::cyan));
  cLeft->setVisible(false);
  cRight->setVisible(false);
  cBehind->setVisible(false);
  QList<QGraphicsItem *> list;
  list.append(agent);
  list.append(cLeft);
  list.append(cRight);
  list.append(cBehind);
  group = scene.createItemGroup(list);
  group->setScale(0.014);
  group->setBoundingRegionGranularity(1.);
  group->setPos(loop->getAgentPositionX(), loop->getAgentPositionY());
  group->setTransformOriginPoint(0.1, -0.45);
  turnSlot(loop->getAgentAngle());
  view->sceneFitInView();
}

void EnvironmentSmallLoopWidget::moveSlot(const unsigned int position,
                                      const int result)
{
  clearLast();
  if(result == EnvironmentSmallLoop::WALL)
    {
      cBehind->setBrush(QBrush(Qt::red));
    }
  else
    {
      cBehind->setBrush(QBrush(Qt::white));
      group->setPos((position % X), position / X);
    }
  lastFeel = cBehind;
  lastFeel->setVisible(true);
}

void EnvironmentSmallLoopWidget::turnSlot(
    const int angle)
{
  EnvironmentSmallLoop::ANGLE a = (EnvironmentSmallLoop::ANGLE)angle;
  clearLast();
  QTransform t;

  switch(a)
    {
    case EnvironmentSmallLoop::NORTH:
      t.translate(0., -0.5);
      t.rotate(180);
      t.translate(-1., -0.5);
      break;
    case EnvironmentSmallLoop::SOUTH:
      t.rotate(0);
      break;
    case EnvironmentSmallLoop::EAST:
      t.rotate(270);
      t.translate(-0.5, 0.5);
      break;
    case EnvironmentSmallLoop::WEST:
      t.rotate(90);
      t.translate(-0.5, -0.5);
      break;
    }
  group->setTransform(t);
}

void EnvironmentSmallLoopWidget::feelSlot(const int result, const int dir)
{
  clearLast();
  switch((EnvironmentSmallLoop::DIRECTION)dir)
    {
    case EnvironmentSmallLoop::LEFT:
      lastFeel = cLeft;
      break;
    case EnvironmentSmallLoop::RIGHT:
      lastFeel = cRight;
      break;
    case EnvironmentSmallLoop::BEHIND:
      lastFeel = cBehind;
      break;
    }
  if(result == EnvironmentSmallLoop::WALL)
    {
      lastFeel->setBrush(QBrush(Qt::darkGreen));
    }
  else
    {
        lastFeel->setBrush(QBrush(Qt::green));
    }
  lastFeel->setVisible(true);
}

void EnvironmentSmallLoopWidget::mouseClickOnScene(const QPointF &point)
{
  for(QGraphicsItem * item : scene.items(point, Qt::IntersectsItemShape))
    {
      if(item != agent && item != cLeft && item != cRight && item != cBehind
         && item != group)
        {
          QGraphicsRectItem *rect = static_cast<QGraphicsRectItem *>(item);
          if(rect->pen().color() == Qt::black)
            rect->setPen(QPen(Qt::white));
          else
            rect->setPen(QPen(Qt::black));
          emit changeItem(boards.indexOf(rect));
        }
    }
}

void EnvironmentSmallLoopWidget::clearLast()
{
  if(lastFeel != NULL)
    {
      lastFeel->setBrush(QBrush(Qt::blue));
      lastFeel->setVisible(false);
      lastFeel = NULL;
    }
}
