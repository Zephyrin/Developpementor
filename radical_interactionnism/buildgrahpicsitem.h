#ifndef BUILDGRAHPICSITEM_H
#define BUILDGRAHPICSITEM_H

#include <usagetable.hpp>

#include <QObject>
#include <QGraphicsItem>
#include <QPen>
#include <QGraphicsScene>

class BuildGrahpicsItem : public QObject
{
  Q_OBJECT
public:
  explicit BuildGrahpicsItem();
  ~BuildGrahpicsItem();
  enum FIGURE{
    SPHERE,
    RECT,
    TRIANGLE,
    LOSDOWN,
    LOSUP,
    TRIANGLE_LEFT
    , TRIANGLE_RIGHT
    , TRIANGLE_BEHIND
    , TRIANGLE_LEFT_SQUARE
    , TRIANGLE_RIGHT_SQUARE
    , TRIANGLE_BEHIND_SQUARE
  };

  void centerBInA(QGraphicsItem *itemA, QGraphicsItem *itemB) const ;

  QGraphicsItem *createExperience(
      QGraphicsScene &scene
      , const std::string &id) const;

  void addTextToItem(const std::string &text, QGraphicsItem *item) const;

  const QColor &getColorOfFigures(const std::string &id) const;
  const QColor &getColorOfText(const std::string &id) const;

  QGraphicsPolygonItem *drawAgent(QGraphicsScene &scene) const;

  QGraphicsRectItem *drawLeftRectAgent(QGraphicsScene &scene) const;

  QGraphicsRectItem *drawRightRectAgent(QGraphicsScene &scene) const;

  QGraphicsRectItem *drawBehindRectAgent(QGraphicsScene &scene) const;

  QGraphicsItemGroup *createAgentGroupAndScale(
      QGraphicsItem *agent
      , QGraphicsRectItem *leftItem
      , QGraphicsRectItem *rightItem
      , QGraphicsRectItem *behindItem
      , QGraphicsScene &scene) const;
  void paintPhenomena(QPainter *painter, const UsageTable *noema) const;
  void paintExperience(QPainter *painter, const std::string &id, qreal x, qreal y) const;
  void paintUnknownPhenomena(QPainter *painter, const QList<const UsageTable *> &noemas) const;
public slots:
  void changeFiguresDefinition(
      const std::map<std::string, FIGURE> &figuresExperiences
      , const std::map<std::string, QColor> &figuresResults);

protected:
  std::map<std::string, FIGURE> figuresExperiences;
  std::map<std::string, QColor> figuresResults;
};

#endif // BUILDGRAHPICSITEM_H
