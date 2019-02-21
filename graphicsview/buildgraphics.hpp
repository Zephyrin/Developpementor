#ifndef BUILDGRAPHICS_HPP
#define BUILDGRAPHICS_HPP

#include <map>
#include <QString>
#include <QGraphicsItem>

class UsageTable;

class BuildGraphics
{
public:
  BuildGraphics();
  ~BuildGraphics();

  enum FIGURE{
    SPHERE
    , RECT
    , TRIANGLE
    , LOSDOWN
    , LOSUP
    , TRIANGLE_LEFT
    , TRIANGLE_RIGHT
    , TRIANGLE_BEHIND
    , TRIANGLE_LEFT_SQUARE
    , TRIANGLE_RIGHT_SQUARE
    , TRIANGLE_BEHIND_SQUARE
    , COMPOSITE
  };

  void readSettings(const QString &filename);

  void centerBInA(QGraphicsItem *itemA, QGraphicsItem *itemB) const ;

  QGraphicsItem *createExperience(
      QGraphicsScene &scene
      , const std::string &id
      , const qreal width = 100.
      , const qreal height = 100.) const;

  void addTextToItem(const std::string &text
                     , QGraphicsItem *item) const;

  const QColor getColorOfFigures(const std::string &id) const;
  const QColor getColorOfText(const std::string &id) const;

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

  QRectF paintPhenomena(QPainter *painter
                      , const UsageTable *noema) const;
  void paintExperience(QPainter *painter
                       , const std::string &id
                       , qreal x
                       , qreal y) const;
  void paintUnknownPhenomena(QPainter *painter
                             , const QList<const UsageTable *> &noemas) const;

  void paintUsageTable(QPainter *painter, const UsageTable *noema) const;
  void paintUsage(QPainter *painter
                  , const int nbEnacted
                  , const int nbIntended
                  , const qreal x
                  , const qreal y) const;
  void paintText(QPainter *painter
                 , const QString &text
                 , const qreal x
                 , const qreal y) const;
protected:
  std::map<std::string, BuildGraphics::FIGURE> figures_;
  QMap<std::string, QString> colors_;
};

#endif // BUILDGRAPHICS_HPP
