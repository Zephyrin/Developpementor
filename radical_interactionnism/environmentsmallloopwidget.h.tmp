#include "environmentstringwidget.h"
#include "ui_environmentstringwidget.h"
#include <QDebug>
#include <QTimer>

EnvironmentStringWidget::EnvironmentStringWidget(const BuildGraphics &build
    , EnvironmentString *string
    , QWidget *parent)
  :
    WidgetGraphicsView(build, "envString", parent)
  , string(string)
  , interactionEnacted(NULL)
{
  QObject::connect(string, SIGNAL(feelSignal(const unsigned int,
                                             const Interaction*)),
                   this, SLOT(feelSlot(const unsigned int,
                                       const Interaction*)));
  QObject::connect(string, SIGNAL(stepSignal(const unsigned int,
                                             const Interaction*)),
                   this, SLOT(stepSlot(const unsigned int,
                                       const Interaction*)));
  QObject::connect(string, SIGNAL(swapSignal(const unsigned int,
                                             const unsigned int,
                                             const Interaction*)),
                   this, SLOT(swapSlot(const unsigned int,
                                       const unsigned int,
                                       const Interaction*)));
  connect(string, SIGNAL(initFinished()), this, SLOT(init()));
}

EnvironmentStringWidget::~EnvironmentStringWidget()
{
}

void EnvironmentStringWidget::init()
{
  const std::vector<int> &board = string->getBoard();
  max = board.at(0);
  for(int i : board)
    if(i > max)
      max = i;
  max += 2;
  view->scene()->setSceneRect(0., 0., 0., 0.);
  for(std::size_t i = 0; i < board.size(); i ++)
    {
      QGraphicsRectItem *line = view->scene()->addRect(0, 0, 100,
                                              max - (100 * (board.at(i) + 1)),
                                              QPen(Qt::black, 10),
                                              QBrush(QColor(Qt::black)));
      QGraphicsTextItem *text = view->scene()->addText(
            QString::number(board.at(i)));
      text->setTextWidth(line->boundingRect().width());
      QFontMetrics fm(text->font());
      float factor = line->boundingRect().width()
          / (fm.boundingRect(QString::number(board.at(i))).width() + 10);
      text->setScale(factor);
      text->setPos(0, line->boundingRect().top() + 5);
      QList<QGraphicsItem *> items;
      items.append(line);
      items.append(text);
      this->boardGraphics.push_back(view->scene()->createItemGroup(items));
      this->boardGraphics[i]->setPos(i * 200, 0);
    }
  agent = build.drawAgent(*(view->scene()));
  interactionEnacted = NULL;
  QTransform t;
  t.rotate(270);
  t.translate(-0.5, -0.5);
  agent->setTransform(t);
  setAgentPos(string->getAgentPosition());
  view->scene()->setSceneRect(view->scene()->itemsBoundingRect().x() - 10
                     , view->scene()->itemsBoundingRect().y()
                     - agent->boundingRect().height() * 2
                     , board.size() * 200 + 20
                     , view->scene()->itemsBoundingRect().height()
                     + agent->boundingRect().height() * 2);
  view->scene()->addRect(view->scene()->sceneRect());
  QTimer::singleShot(1, this, SLOT(fitInView()));
}

void EnvironmentStringWidget::stepSlot(const unsigned int nextPos,
                                       const Interaction *enacted)
{
  resetColor(nextPos);
  setAgentPos(nextPos);
  changeColor(nextPos, build.getColorOfFigures(enacted->getId()));
  setInteractionEnacted(enacted);
}

void EnvironmentStringWidget::feelSlot(const unsigned int nextPos,
                                       const Interaction *enacted)
{
  resetColor(nextPos);
  if(nextPos < (unsigned int)boardGraphics.size())
    {
      changeColor(nextPos, build.getColorOfFigures(enacted->getId()));
    }
  else
    {
      changeColor(nextPos - 1, build.getColorOfFigures(enacted->getId()));
    }
  setInteractionEnacted(enacted);
}

void EnvironmentStringWidget::swapSlot(const unsigned int pos,
                                       const unsigned int nextPos,
                                       const Interaction *enacted)
{
  resetColor(nextPos);
  if(pos < nextPos)
    {
      QGraphicsItemGroup *item1 = boardGraphics[pos];
      QGraphicsItemGroup *item2 = boardGraphics[nextPos];
      QPointF pos1 = item1->pos();
      item1->setPos(item2->pos());
      item2->setPos(pos1);
      boardGraphics[pos] = item2;
      boardGraphics[nextPos] = item1;
      setAgentPos(pos);
      changeColor(pos, build.getColorOfFigures(enacted->getId()));
      changeColor(nextPos, build.getColorOfFigures(enacted->getId()));
    }
  else
    {
      changeColor(pos, build.getColorOfFigures(enacted->getId()));
      changeColor(0, build.getColorOfFigures(enacted->getId()));
    }
  setInteractionEnacted(enacted);
}

void EnvironmentStringWidget::resetColor(const int nextPos)
{
  if(nextPos > 0)
    changeColor(nextPos - 1, QColor(Qt::black));
  else
    changeColor(boardGraphics.size() - 1, QColor(Qt::black));
  if(nextPos == boardGraphics.size())
    changeColor(0, QColor(Qt::black));
  else
    changeColor(nextPos, QColor(Qt::black));
}

void EnvironmentStringWidget::changeColor(const int index, const QColor &color)
{
  ((QGraphicsRectItem *)boardGraphics[index]->childItems().first())
    ->setBrush(color);
  view->scene()->update();
}

void EnvironmentStringWidget::setAgentPos(const int index)
{
  agent->setPos(
        boardGraphics[index]->pos().x()
        , boardGraphics[index]->pos().y()
        - boardGraphics[index]->boundingRect().height()
        + agent->boundingRect().height()
        );
}

void EnvironmentStringWidget::setInteractionEnacted(const Interaction *enacted)
{
  if(interactionEnacted)
    view->scene()->removeItem(interactionEnacted);
  interactionEnacted = build.createExperience(*(view->scene()),
                                              enacted->getId());
  interactionEnacted->setScale(0.5);
  interactionEnacted->setPos(
        agent->pos().x() + agent->boundingRect().width() + 15
        , agent->pos().y() - agent->boundingRect().height() - 5);
  if(enacted->getValence() == 0)
  agent->setBrush(Qt::blue);
  else if(enacted->getValence() > 0)
    agent->setBrush(build.getColorOfFigures("positiveValence"));
  else
    agent->setBrush(build.getColorOfFigures("negativeValence"));
}

void EnvironmentStringWidget::on_resetBoard_clicked()
{
  boardGraphics.clear();
  view->scene()->clear();
  max = 0;
  string->resetEnv();
  init();
}

